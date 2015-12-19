//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_SERIAL_H
#define ARCTOS_ROBOT_SERIAL_H

#include <termios.h>

int set_interface_attribs(int fd, speed_t speed, int parity);

void set_blocking(int fd, int should_block);


#endif //ARCTOS_ROBOT_SERIAL_H
