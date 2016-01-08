//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <functional>
#include <string.h>
#include <sys/types.h>

#include "communication/CommunicationClient.h"
#include "communication/rfid.h"
#include "controller.h"

Controller::Controller(robot_options_t* options, communication::CommunicationClient* client) {
    this->client = client;

    controller_rfid_init = options->callbacks.controller_rfid_init;
    controller_rfid_read = options->callbacks.controller_rfid_read;
    controller_drive_init = options->callbacks.controller_drive_init;
    controller_drive_left = options->callbacks.controller_drive_left;
    controller_drive_right = options->callbacks.controller_drive_right;

    // Initialize the ports
    fd_rfid = controller_rfid_init(&options->serial_port_options_rfid);

    if (this->client != NULL) {
        std::function<void(int,int)> callback = std::bind(&Controller::drive, this, std::placeholders::_1, std::placeholders::_2);
        client->setDriveCallback(callback);
    }
    this->controller_drive_init();
}

void Controller::heartbeat() {
	if(this->client == NULL) {
		return;
	}

    while (!this->shutdown_requested) {
    	if(this->client != NULL) this->client->sendHeartbeat();
    	sleep(1);
    }
}

void Controller::runBluetooth() {
	if(this->client == NULL) {
		return;
	}

    while (!this->shutdown_requested) {
    	this->client->retrieveData();
    }
}

void Controller::runRFID() {
	if(this->fd_rfid < 0) {
		return;
	}

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
	return this->client != NULL ? this->client->sendLocation(location) : false;
}

void Controller::shutdown() {
    this->shutdown_requested = true;
}

void Controller::drive(int left, int right) {
	this->controller_drive_left(left);
	this->controller_drive_right(right);
}
