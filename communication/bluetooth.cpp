//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <string.h>
#include <sys/fcntl.h>
#include "Bluetooth.h"
#include "serial.h"

Bluetooth::Bluetooth(serial_port_options_t* options) {
    fd = open(options->port_name, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        error_message("error %d opening %s: %s", errno, options->port_name, strerror(errno));
        throw "error";
    }

    // Set baud rate, 8n1 (no parity)
    set_interface_attribs(fd, options->speed, 0);
    // Set no blocking
    set_blocking(fd, 0);
}

ssize_t Bluetooth::bluetooth_read(char* buffer, size_t buffer_size) {
    if(fd < 0) {
        return -1;
    }
    return read(fd, buffer, buffer_size);
}

bool Bluetooth::bluetooth_write(const char* data, size_t data_length) {
    if(fd < 0) {
        return false;
    }

    write(fd, data, strlen(data));
    return true;
}
