//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <string.h>
#include "Controller.h"
#include "communication/rfid.h"
#include "communication/protocol/ProtocolLayer.h"

Controller::Controller(robot_options_t* options, ProtocolLayer* protocol) {
    this->protocol = protocol;

    controller_rfid_init = options->callbacks.controller_rfid_init;
    controller_rfid_read = options->callbacks.controller_rfid_read;

    // Initialize the ports
    fd_rfid = controller_rfid_init(&options->serial_port_options_rfid);
}

void Controller::runBluetooth() {
    while (!this->shutdown_requested) {
        char bluetooth_message[BLUETHOOTH_BUFFER_SIZE];
        pdu_t bluetooth_data;
        bluetooth_data.message = bluetooth_message;
        bluetooth_data.length = BLUETHOOTH_BUFFER_SIZE;

        this->protocol->receive(&bluetooth_data);

        if (bluetooth_data.length > 0) {
            // Bluetooth data received
        }
    }
}

void Controller::runRFID() {
    while (!this->shutdown_requested) {
        char rfid_buffer[RFID_LEN];

        // Read from the RFID input device
        ssize_t size_r = controller_rfid_read(fd_rfid, rfid_buffer, RFID_LEN);

        if (size_r > 0) {
            // RFID data received
            location_t location;
            memcpy(location.id, rfid_buffer + 1, RFID_TAG_LENGTH);

            // Update location
            update_location(location);
        }
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

void Controller::shutdown() {
    this->shutdown_requested = true;
}