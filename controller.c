//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <string.h>
#include "controller.h"

#define RFID_BUFFER_SIZE 12
#define BLUETHOOTH_BUFFER_SIZE 1024

static int fd_rfid = -1;
static int fd_bluetooth = -1;
static location_t last_location;

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
        ssize_t size_r = 0, size_b = 0;
        if (fd_rfid >= 0) {
            size_r = controller_rfid_read(fd_rfid, rfid_buffer, RFID_BUFFER_SIZE);
        }
        if (fd_bluetooth >= 0) {
            size_b = controller_bluetooth_read(fd_bluetooth, bluetooth_buffer, BLUETHOOTH_BUFFER_SIZE);
        }

        if(size_r > 0) {
            // RFID data received
            location_t location;
            memcpy(location.id, rfid_buffer + 1, RFID_TAG_LENGTH);

            // Update location
            err = controller_update_location(location);
        }
        if(size_b > 0) {
            // Bluetooth data received
        }

        // Todo: Write location changes via bluetooth
        // Todo: Receive input commands and use them to control the robot
    }
}

int controller_update_location(location_t location) {
    if(controller_compare_locations(&last_location, &location) != 0) {
        // New location
        last_location = location;

        // Notify server
        return controller_notify_server(&last_location);
    }

    return 0;
}

static int controller_compare_locations(location_t* l1, location_t* l2) {
    return strncmp(l1->id, l2->id, RFID_TAG_LENGTH);
}

int controller_notify_server(location_t* location) {
    return controller_bluetooth_write(fd_bluetooth, location->id + '\n');
}
