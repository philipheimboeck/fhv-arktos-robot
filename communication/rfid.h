//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_RFID_H
#define ARCTOS_ROBOT_RFID_H


#include <stddef.h>
#include "../general.h"

int rfid_init(serial_port_options_t* options);

int rfid_read(int fd, char* buffer, size_t buffer_size);

#endif //ARCTOS_ROBOT_RFID_H
