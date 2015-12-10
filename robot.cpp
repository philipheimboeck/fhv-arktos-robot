/*
 * robot.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */

#include "includes/DMCC/DMCC.h"
#include "robot.h"

static int session = -1; 				///> Session received by DMCCstart

/**
 * Calculate velocity of given input (in percentage)
 */
static void set_velocity(int motor, double input) {
	if (input > 100) {
		input = 100;
	} else if (input < -100) {
		input = -100;
	}

    setMotorPower(session, motor, input / 100.0 * VELOCITY_MAX);
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

void robot_turn_left() {
	robot_connect();

    setMotorPower(session, MOTOR_LEFT, VELOCITY_MIN);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_MAX);
}

void robot_turn_right() {
	robot_connect();

    setMotorPower(session, MOTOR_LEFT, VELOCITY_MAX);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_MIN);
}

void robot_stop() {
	robot_connect();

    setMotorPower(session, MOTOR_LEFT, VELOCITY_STOP);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_STOP);
}
