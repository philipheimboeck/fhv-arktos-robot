/*
 * main.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */
#include <stdio.h>
#include "robot.h"

int main() {
    //printf("Test\n");

    robot_init();
    //robot_drive_left(500);

    while(1) {
        // rfid tag read?
        // send bluetooth

        // wait for bluetooth inputs
        // evalute singals
    }

    robot_deconstruct();
    return 0;
}