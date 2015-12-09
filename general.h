//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_GENERAL_H
#define ARCTOS_ROBOT_GENERAL_H

#include <sys/termios.h>

typedef struct {
    char* port_name;
    speed_t speed;
} serial_port_options_t;

void error_message(const char* message, ...);

#endif //ARCTOS_ROBOT_GENERAL_H
