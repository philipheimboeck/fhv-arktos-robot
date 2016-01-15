/*
 * robot.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */

#include <stdlib.h>
#include "robot.h"

#include "includes/DMCC/DMCC.h"

static int session = -1; 				///> Session received by DMCCstart
static const int VELOCITY_DELTA = 2500;	///> Size to increase velocity step for step
static int motor_velocity[3] = {0};

/**
 * Slowly increase velocity of motor
 * Parameter: input - target velocity of motor in percentage
 */
static void slowly_increase_velocity(int motor, int input) {
    int currentVel = motor_velocity[motor];
    int targetVel = input;

    if ( currentVel == targetVel ) {
    	return;
    }

    int difVel = targetVel - currentVel;

    // if value is bigger than delta value
    // increase only delta value
    if ( targetVel > 5000 && abs(difVel) > VELOCITY_DELTA ) {
    	currentVel += (difVel < 0 ? -1 : 1) * VELOCITY_DELTA;
    } else {
    	currentVel = targetVel;
    }
    setMotorPower(session, motor, currentVel);
    motor_velocity[motor] = currentVel;
}

/**
 * Calculate velocity of given input (in percentage)
 */
static void set_velocity(int motor, double input) {
	if (input > 100) {
		input = 100;
	} else if (input < -100) {
		input = -100;
	}

	slowly_increase_velocity(motor, input / 100.0 * VELOCITY_MAX);
}

void robot_init() {
	robot_connect();

    robot_stop();
}

void robot_connect() {
	// check connection
	//TODO: any possibility to check concurrent connection?
	if (session == -1) {
	    session = DMCCstart(BOARD_ID);
	}
}

void robot_disconnect() {
	robot_stop();
    DMCCend(session);
    session = -1;
}

void robot_drive_left(double input) {
	robot_connect();

    set_velocity(MOTOR_LEFT, input);
}

void robot_drive_right(double input) {
	robot_connect();

    set_velocity(MOTOR_RIGHT, input);
}

void robot_stop() {
	robot_connect();

	robot_drive_left(0);
	robot_drive_right(0);
}
