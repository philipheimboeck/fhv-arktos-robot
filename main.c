/*
 * main.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <string.h>
#include "robot.h"
#include "communication/serial.h"
#include "general.h"

void testSerial(const char * port);

int main(int argc, char* argv[]) {
    printf("Init\n");

    char* port = "/dev/pts/6";
    printf("Number of arguments: %d\n", argc);
    if(argc > 1) {
        port = argv[1];
    }

    //robot_init();
    //robot_drive_left(250);
    //robot_turn_left();

    printf("Using port: %s\n", port);
    testSerial(port);

    while (1) {
        // rfid tag read?
        // send bluetooth

        // wait for bluetooth inputs
        // evalute singals
    }

    robot_disconnect();
    return 0;
}

void testSerial(const char * port) {

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