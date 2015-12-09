//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_RFID_H
#define ARCTOS_ROBOT_RFID_H

#include <unistd.h>
#include <stddef.h>
#include "../general.h"

#define START_BYTE	0x0A	///< '\n'
#define STOP_BYTE	0x0D	///< '\r'
#define RFID_LEN	12     	///< 10 bytes of RFID tag + between START_BYTE and STOP_BYTE

int rfid_init(serial_port_options_t* options);

ssize_t rfid_read(int fd, char* buffer, size_t buffer_size);

#endif //ARCTOS_ROBOT_RFID_H
