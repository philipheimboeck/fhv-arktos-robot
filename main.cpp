/*
 * main.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */

#include <pthread.h>
#include <termios.h>
#include <cstdio>
#include <string>
#include <signal.h>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <ctime>

#include "communication/CommunicationClient.h"
#include "communication/rfid.h"
#include "controller.h"
#include "general.h"
#include "robot.h"

Controller* controller;

void start(Controller* controller);

void* thread_bluetooth_main(void* controller) {
    printf("Starting bluetooth thread...\n");
    ((Controller*) controller)->runBluetooth();
    printf("Exiting bluetooth thread...\n");
    pthread_exit(NULL);
}

void* thread_heartbeat_main(void* controller) {
    printf("Starting heartbeat thread...\n");
    ((Controller*) controller)->heartbeat();
    printf("Exiting heartbeat thread...\n");
    pthread_exit(NULL);
}

void* thread_rfid_main(void* controller) {
    printf("Starting RFID thread...\n");
    ((Controller*) controller)->runRFID();
    printf("Exiting RFID thread...\n");
    pthread_exit(NULL);
}

void signal_handler(int signal) {
	// get current time
	std::time_t time = std::time(NULL);
	char time_buffer[100];
	std::strftime(time_buffer, sizeof(time_buffer), "%d.%m.%Y %H:%M:%S", std::localtime(&time));

	std::cerr << time_buffer << ": Error: signal " << signal << std::endl;

	if(controller != NULL) {
		printf("Shutting down controller...\n");
		controller->shutdown();
	}

	std::abort();
}

void termination_handler() {
	// get current time
	std::time_t time = std::time(NULL);
	char time_buffer[100];
	std::strftime(time_buffer, sizeof(time_buffer), "%d.%m.%Y %H:%M:%S", std::localtime(&time));

	std::exception_ptr exptr = std::current_exception();
    if (exptr != 0) {
		try {
			std::rethrow_exception(exptr);
		}
		catch (std::exception &ex) {
			std::cerr << time_buffer << ": Terminated due to exception: " << ex.what() << std::endl;
		}
    } else {
        std::cerr << time_buffer << ": Terminated due to unknown reason" << std::endl;
    }

	if(controller != NULL) {
		printf("Shutting down controller...\n");
		controller->shutdown();
	}

    std::abort();
}

int main(int argc, char* argv[]) {
	// Register signal handler
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
    signal(SIGSEGV, signal_handler);
    std::set_terminate(termination_handler);

	// Redirect cerr to log file
	std::fstream error_log;
	error_log.open("/var/log/arctos_error.log", std::fstream::app|std::fstream::out);
	std::streambuf* error_buffer = std::cerr.rdbuf(error_log.rdbuf());

	// Get the input params
    std::string bluetooth_port = "/dev/rfcomm0";
    std::string rfid_port = "/dev/ttyUSB0";
    if (argc > 1) {
        bluetooth_port = argv[1];
    }
    if (argc > 2) {
        rfid_port = argv[2];
    }

    robot_options_t options;
    options.callbacks.controller_rfid_init = rfid_init;
    options.callbacks.controller_rfid_read = rfid_read;
    options.callbacks.controller_drive_init = robot_init;
    options.callbacks.controller_drive_left = robot_drive_left;
    options.callbacks.controller_drive_right = robot_drive_right;
    options.serial_port_options_rfid.port_name = rfid_port;
    options.serial_port_options_rfid.speed = B2400;

    serial_port_options_t bluetooth_options;
    bluetooth_options.port_name = bluetooth_port;
    bluetooth_options.speed = B9600;

    communication::CommunicationClient* client = NULL;
    try {
    	client = new communication::CommunicationClient(bluetooth_options);
        controller = new Controller(&options, client);

        // Start threads
        start(controller);
    } catch (int error) {
    	printf("Could not create communication client: %d\n", error);
    }

    // Exit process
    delete (controller);
    delete (client);

    // Stop the robot
    robot_stop();

    // Restore the cerr stream
    std::cerr.rdbuf(error_buffer);
    error_log.close();

    return 0;
}

void start(Controller* controller) {
    pthread_t thread_bluetooth, thread_rfid, thread_heartbeat;
    pthread_create(&thread_rfid, NULL, thread_rfid_main, controller);
    pthread_create(&thread_bluetooth, NULL, thread_bluetooth_main, controller);
    pthread_create(&thread_heartbeat, NULL, thread_heartbeat_main, controller);

    // Wait for threads to finish
    void* status;
    pthread_join(thread_bluetooth, &status);
    pthread_join(thread_rfid, &status);
    pthread_join(thread_heartbeat, &status);
}

