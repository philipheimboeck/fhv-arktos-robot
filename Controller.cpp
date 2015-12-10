//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <string.h>
#include "Controller.h"

#define RFID_BUFFER_SIZE 12

Controller::Controller(robot_options_t* options, ProtocolLayer* protocol) {
    this->protocol = protocol;

    controller_rfid_init = options->callbacks.controller_rfid_init;
    controller_rfid_read = options->callbacks.controller_rfid_read;

    // Initialize the ports
    fd_rfid = controller_rfid_init(&options->serial_port_options_rfid);
}


void Controller::start() {
    char rfid_buffer[RFID_BUFFER_SIZE];
    int err = 0;

    // Loop while no error occurs
    while (err == 0) {
        // Read from the inputs
        ssize_t size_r = controller_rfid_read(fd_rfid, rfid_buffer, RFID_BUFFER_SIZE);
        pdu_t bluetooth_data;
        this->protocol->receive(&bluetooth_data);

        if (size_r > 0) {
            // RFID data received
            location_t location;
            memcpy(location.id, rfid_buffer + 1, RFID_TAG_LENGTH);

            // Update location
            err = update_location(location);
        }

        if (bluetooth_data.length > 0) {
            // Bluetooth data received
        }

        // Todo: Write location changes via bluetooth
        // Todo: Receive input commands and use them to control the robot
    }
}

bool Controller::update_location(location_t location) {
    if (compare_locations(&last_location, &location) != 0) {
        // New location
        last_location = location;

        // Notify server
        return notify_server(&last_location);
    }

    return false;
}

int Controller::compare_locations(location_t* l1, location_t* l2) {
    return strncmp(l1->id, l2->id, RFID_TAG_LENGTH);
}

bool Controller::notify_server(location_t* location) {
    pdu_t pdu;
    pdu.message = location->id;
    pdu.length = RFID_TAG_LENGTH;
    return this->protocol->send(&pdu);
}

