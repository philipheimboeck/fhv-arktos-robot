//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <sys/fcntl.h>
#include <sys/errno.h>
#include <string.h>
#include "bluetooth.h"
#include "serial.h"

int bluetooth_init(serial_port_options_t* options) {
    int fd = open(options->port_name, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        error_message("error %d opening %s: %s", errno, options->port_name, strerror(errno));
        return -1;
    }

    // Set baud rate, 8n1 (no parity)
    set_interface_attribs(fd, options->speed, 0);
    // Set no blocking
    set_blocking(fd, 0);

    return fd;
}

ssize_t bluetooth_read(int fd, char* buffer, size_t buffer_size) {
    return read(fd, buffer, buffer_size);
}

int bluetooth_write(int fd, char* data) {
    write(fd, data, strlen(data));
    return 0;
}


