//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <sys/fcntl.h>
#include <sys/errno.h>
#include <string.h>
#include "bluetooth.h"
#include "serial.h"

void testSerial(const char* port) {

    int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        error_message("error %d opening %s: %s", errno, port, strerror(errno));
        return;
    }

    set_interface_attribs(fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd, 0);                // set no blocking

    write(fd, "hello!\n", 7);           // send 7 character greeting

    usleep((7 + 25) * 100);             // sleep enough to transmit the 7 plus
    // receive 25:  approx 100 uS per char transmit
    char buf[100];
    ssize_t n = read(fd, buf, sizeof buf);  // read up to 100 characters if ready to read
}

int bluetooth_init(serial_port_options_t* options) {
    testSerial(options->port_name);
    return 1;
}

int bluetooth_read(int fd, char* buffer, size_t buffer_size) {
    // Todo: Implement
    return 1;
}

int bluetooth_write(int fd, char* data) {
    // Todo: Implement
    return 1;
}


