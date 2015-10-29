/*
 * robot.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */

#include "includes/DMCC/DMCC.h"
#include "robot.h"

static const int VELOCITY_STEP = 250; ///> Size to increase velocity step for step
static int session = -1; ///> Session received by DMCCstart

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
    int highestControllerInput = 250;
    int vel = input / highestControllerInput * VELOCITY_MAX;
    return slowly_increase_velocity(motor, vel);
}

void robot_init() {
    session = DMCCstart(BOARD_ID);

    robot_stop();
}

void robot_deconstruct() {
    DMCCend(session);
}

void robot_drive_left(int input) {
    set_velocity(MOTOR_LEFT, input);
}

void robot_drive_right(int input) {
    set_velocity(MOTOR_RIGHT, input);
}

void robot_turn_left() {
    //robot_stop();
    setMotorPower(session, MOTOR_LEFT, VELOCITY_MIN);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_MAX);
}

void robot_turn_right() {
    //robot_stop();
    setMotorPower(session, MOTOR_LEFT, VELOCITY_MAX);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_MIN);
}

void robot_stop() {
    setMotorPower(session, MOTOR_LEFT, VELOCITY_STOP);
    setMotorPower(session, MOTOR_RIGHT, VELOCITY_STOP);
}
