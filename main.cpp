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

#include "communication/CommunicationClient.h"
#include "communication/rfid.h"
#include "controller.h"
#include "general.h"
#include "robot.h"

void start(Controller* controller);

void* thread_bluetooth_main(void* controller) {
    printf("Starting bluetooth thread...\n");
    ((Controller*) controller)->runBluetooth();
    pthread_exit(NULL);
}

void* thread_rfid_main(void* controller) {
    printf("Starting RFID thread...\n");
    ((Controller*) controller)->runRFID();
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

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

    communication::CommunicationClient* client = new communication::CommunicationClient(bluetooth_options);
    Controller* controller = new Controller(&options, client);

    // Start threads
    start(controller);

    delete (controller);
    delete (client);

    return 0;
}

void start(Controller* controller) {
    pthread_t thread_bluetooth, thread_rfid;
    pthread_create(&thread_rfid, NULL, thread_rfid_main, controller);
    pthread_create(&thread_bluetooth, NULL, thread_bluetooth_main, controller);

    while(1) {
    }

    pthread_exit(NULL);
}

