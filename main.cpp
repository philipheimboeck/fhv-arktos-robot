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

void* thread_rfid_main(void* controller) {
    printf("Starting RFID thread...\n");
    ((Controller*) controller)->runRFID();
    printf("Exiting RFID thread...\n");
    pthread_exit(NULL);
}

void signal_handler(int signal) {
	if(controller != NULL) {
		printf("Shutting down controller...\n");
		controller->shutdown();
	}
}

int main(int argc, char* argv[]) {
	// Register signal handler
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);

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
    } catch (int error) {
    	printf("Could not create communication client: %d\n", error);
    }

    controller = new Controller(&options, client);

    // Start threads
    start(controller);

    // Exit process
    delete (controller);
    delete (client);

    // Stop the robot
    robot_drive_left(0);
    robot_drive_right(0);

    return 0;
}

void start(Controller* controller) {
    pthread_t thread_bluetooth, thread_rfid;
    pthread_create(&thread_rfid, NULL, thread_rfid_main, controller);
    pthread_create(&thread_bluetooth, NULL, thread_bluetooth_main, controller);

    // Wait for threads to finish
    void* status;
    pthread_join(thread_bluetooth, &status);
    pthread_join(thread_rfid, &status);
}

