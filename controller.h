//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_CONTROLLER_H
#define ARCTOS_ROBOT_CONTROLLER_H

#include <stdio.h>
#include "general.h"
#include "communication/protocol/ProtocolLayer.h"

#define RFID_TAG_LENGTH 10

typedef struct {
    int (* controller_rfid_init)(serial_port_options_t* options);

    ssize_t (* controller_rfid_read)(int fd, char*, size_t);
} robot_callbacks_t;

typedef struct {
    serial_port_options_t serial_port_options_rfid;
    robot_callbacks_t callbacks;
} robot_options_t;

typedef struct {
    char id[RFID_TAG_LENGTH];
} location_t;

class Controller {
private:
    int fd_rfid;
    location_t last_location;

    ProtocolLayer* protocol;

    int compare_locations(location_t* l1, location_t* l2);

    bool notify_server(location_t* location);

    bool update_location(location_t location);

    /* Callbacks */

    /**
     * Returns a file descriptor
     */
    int (* controller_rfid_init)(serial_port_options_t* options);

    /**
     * Reads the input and saves it into the buffer
     */
    ssize_t (* controller_rfid_read)(int fd, char*, size_t);

public:
    Controller(robot_options_t* options, ProtocolLayer* protocol);

    void start();

};

#endif //ARCTOS_ROBOT_CONTROLLER_H
