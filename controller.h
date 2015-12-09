//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_CONTROLLER_H
#define ARCTOS_ROBOT_CONTROLLER_H

#include <stddef.h>
#include "general.h"

typedef struct {
    int (*controller_rfid_init)(serial_port_options_t* options);

    int (*controller_rfid_read)(int fd, char*, size_t);

    int (*controller_bluetooth_init)(serial_port_options_t* options);

    int (*controller_bluetooth_read)(int fd, char*, size_t);

    int (*controller_bluetooth_write)(int fd, char* data);
} robot_callbacks_t;

typedef struct {
    serial_port_options_t serial_port_options_bluetooth;
    serial_port_options_t serial_port_options_rfid;
    robot_callbacks_t callbacks;
} robot_options_t;

void controller_init(robot_options_t options);

void controller_start();


/* Callbacks */


/**
 * Returns a file descriptor
 */
int (*controller_rfid_init)(serial_port_options_t* options);

/**
 * Reads the input and saves it into the buffer
 */
int (*controller_rfid_read)(int fd, char*, size_t);

/**
 * Returns a file descriptor
 */
int (*controller_bluetooth_init)(serial_port_options_t* options);

/**
 * Reads the input and saves it into the buffer
 */
int (*controller_bluetooth_read)(int fd, char*, size_t);

/**
 * Sends data through the device
 */
int (*controller_bluetooth_write)(int fd, char* data);

#endif //ARCTOS_ROBOT_CONTROLLER_H
