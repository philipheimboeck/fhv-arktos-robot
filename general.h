//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_GENERAL_H
#define ARCTOS_ROBOT_GENERAL_H

#include <string>
#include <termios.h>

#define RFID_TAG_LENGTH 10

typedef struct {
    std::string port_name;
    speed_t speed;
} serial_port_options_t;

typedef struct {
    char id[RFID_TAG_LENGTH];
} location_t;

void error_message(std::string message, ...);

#endif //ARCTOS_ROBOT_GENERAL_H
