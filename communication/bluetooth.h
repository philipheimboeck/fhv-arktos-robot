//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_BLUETOOTH_H
#define ARCTOS_ROBOT_BLUETOOTH_H

#include <stddef.h>
#include "../general.h"

int bluetooth_init(serial_port_options_t* options);

int bluetooth_read(int fd, char* buffer, size_t buffer_size);

int bluetooth_write(int fd, char* data);

#endif //ARCTOS_ROBOT_BLUETOOTH_H
