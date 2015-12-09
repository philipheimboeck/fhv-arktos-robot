//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include "controller.h"

#define RFID_BUFFER_SIZE 32
#define BLUETHOOTH_BUFFER_SIZE 1024

static int fd_rfid;
static int fd_bluetooth;

void controller_init(robot_options_t options) {
    controller_bluetooth_init = options.callbacks.controller_bluetooth_init;
    controller_bluetooth_read = options.callbacks.controller_bluetooth_read;
    controller_bluetooth_write = options.callbacks.controller_bluetooth_write;
    controller_rfid_init = options.callbacks.controller_rfid_init;
    controller_rfid_read = options.callbacks.controller_rfid_read;

    // Initialize the ports
    fd_rfid = controller_rfid_init(&options.serial_port_options_rfid);
    fd_bluetooth = controller_bluetooth_init(&options.serial_port_options_bluetooth);
}

void controller_start() {
    char rfid_buffer[RFID_BUFFER_SIZE];
    char bluetooth_buffer[BLUETHOOTH_BUFFER_SIZE];
    int err = 0;

    // Loop while no error occurs
    while (err == 0) {
        // Read from the inputs
        err += controller_rfid_read(fd_rfid, rfid_buffer, RFID_BUFFER_SIZE);
        err += controller_bluetooth_read(fd_bluetooth, bluetooth_buffer, BLUETHOOTH_BUFFER_SIZE);

        // Todo: Write location changes via bluetooth
        // Todo: Receive input commands and use them to control the robot
    }
}