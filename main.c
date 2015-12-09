/*
 * main.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <string.h>
#include "robot.h"
#include "communication/serial.h"
#include "communication/bluetooth.h"
#include "general.h"
#include "controller.h"
#include "communication/rfid.h"

void testSerial(const char* port);

int main(int argc, char* argv[]) {

    // Get the input params
    char* bluetooth_port = "/dev/pts/6";
    char* rfid_port = "/dev/ttyUSB0";
    if (argc > 1) {
        bluetooth_port = argv[1];
    }
    if (argc > 2) {
        rfid_port = argv[2];
    }

    //robot_init();
    //robot_drive_left(250);
    //robot_turn_left();

    robot_options_t options;
    options.callbacks.controller_bluetooth_init = bluetooth_init;
    options.callbacks.controller_bluetooth_read = bluetooth_read;
    options.callbacks.controller_bluetooth_write = bluetooth_write;
    options.callbacks.controller_rfid_init = rfid_init;
    options.callbacks.controller_rfid_read = rfid_read;
    options.serial_port_options_bluetooth.port_name = bluetooth_port;
    options.serial_port_options_bluetooth.speed = B115200;
    options.serial_port_options_rfid.port_name = rfid_port;
    options.serial_port_options_rfid.speed = B115200;

    controller_init(options);
    controller_start();

    //robot_disconnect();
    return 0;
}