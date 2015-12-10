/*
 * robot.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */

#include "includes/DMCC/DMCC.h"
#include "robot.h"

static const int VELOCITY_STEP = 250; 	///> Size to increase velocity step for step
static int session = -1; 				///> Session received by DMCCstart

/**
 * Slowly increase velocity of motor
 * too lower increase of current intensity
 */
static void slowly_increase_velocity(int motor, int pwm) {
    int currentVel = getTargetVel(session, motor);
    int targetVel = pwm; //TODO: safe clause of too high or too low?

    int difVel = targetVel - currentVel;
    int temp = 0;
    // slowly increase speed based on VELOCITY_STEP
    while (difVel != 0) {
        temp = difVel % VELOCITY_STEP;
        if (temp == 0) {
            difVel -= VELOCITY_STEP;
            currentVel += VELOCITY_STEP;
        } else {
            difVel -= temp;
            currentVel += temp;
        }

        setMotorPower(session, motor, currentVel);
        //TODO: sleep some ms
    }
}

/**
 * Calculate velocity of given input
 * based on highest input of controller
 */
static void set_velocity(int motor, int input) {
    //TODO: integrate highest input of controller
    double highestControllerInput = 500;
    double velRatio = input / highestControllerInput;
    if (velRatio > 1) {
    	velRatio = 1;
    }

    return slowly_increase_velocity(motor, velRatio * VELOCITY_MAX);
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

void robot_drive_left(int input) {
	robot_connect();

    set_velocity(MOTOR_LEFT, input);
}

void robot_drive_right(int input) {
	robot_connect();

    set_velocity(MOTOR_RIGHT, input);
}

void robot_turn_left() {
	robot_connect();

    //robot_stop();
    setMotorPower(session, MOTOR_LEFT, VELOCITY_MIN);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_MAX);
}

void robot_turn_right() {
	robot_connect();

    //robot_stop();
    setMotorPower(session, MOTOR_LEFT, VELOCITY_MAX);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_MIN);
}

void robot_stop() {
	robot_connect();

    setMotorPower(session, MOTOR_LEFT, VELOCITY_STOP);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_STOP);
}
