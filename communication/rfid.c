//
// Created by Philip Heimböck (Privat) on 09.12.15.
//


#include <unistd.h>
#include "rfid.h"

int rfid_init(serial_port_options_t* options) {
    // Todo: Implement
    return 1;
}

ssize_t rfid_read(int fd, char* buffer, size_t buffer_size) {
    return read(fd, buffer, buffer_size);
}