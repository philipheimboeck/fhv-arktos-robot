//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_CONTROLLER_H
#define ARCTOS_ROBOT_CONTROLLER_H

#include <stddef.h>
#include "general.h"
#include <unistd.h>

#define RFID_TAG_LENGTH 10

typedef struct {
    int (*controller_rfid_init)(serial_port_options_t* options);

    ssize_t (*controller_rfid_read)(int fd, char*, size_t);

    int (*controller_bluetooth_init)(serial_port_options_t* options);

    ssize_t (*controller_bluetooth_read)(int fd, char*, size_t);

    int (*controller_bluetooth_write)(int fd, char* data);
} robot_callbacks_t;

typedef struct {
    serial_port_options_t serial_port_options_bluetooth;
    serial_port_options_t serial_port_options_rfid;
    robot_callbacks_t callbacks;
} robot_options_t;

typedef struct {
    char id[RFID_TAG_LENGTH];
} location_t;

void controller_init(robot_options_t options);

void controller_start();

int controller_update_location(location_t location);

int controller_notify_server(location_t* location);

static int controller_compare_locations(location_t* l1, location_t* l2);

/* Callbacks */


/**
 * Returns a file descriptor
 */
int (*controller_rfid_init)(serial_port_options_t* options);

/**
 * Reads the input and saves it into the buffer
 */
ssize_t (*controller_rfid_read)(int fd, char*, size_t);

/**
 * Returns a file descriptor
 */
int (*controller_bluetooth_init)(serial_port_options_t* options);

/**
 * Reads the input and saves it into the buffer
 */
ssize_t (*controller_bluetooth_read)(int fd, char*, size_t);

/**
 * Sends data through the device
 */
int (*controller_bluetooth_write)(int fd, char* data);

#endif //ARCTOS_ROBOT_CONTROLLER_H
