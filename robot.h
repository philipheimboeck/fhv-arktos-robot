/*
 * robot.h
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */

#ifndef ARCTOS_ROBOT_ROBOT_H
#define ARCTOS_ROBOT_ROBOT_H

const int BOARD_ID = 0;         ///> ID of board
const int MOTOR_LEFT = 1;       ///> ID of left motor
const int MOTOR_RIGHT = 2;      ///> ID of right motor

const int VELOCITY_MAX = 3000;  ///> Max velocity of robot
const int VELOCITY_MIN = -3000; ///> Min velocity of robot
const int VELOCITY_STOP = 0;    ///> Stop velocity of robot

/**
 * Initialises robot
 */
void robot_init();

/**
 * Turn off robot
 */
void robot_deconstruct();

/**
 * Set left motor to given velocity in ratio to max velocity
 */
void robot_drive_left(int input);

/**
 * Set right motor to given velocity in ratio to max velocity
 */
void robot_drive_right(int input);

/**
 * Set left motor to min speed
 * and right motor to max speed
 */
void robot_turn_left();

/**
 * Set left motor to max velocity
 * and right motor to min velocity
 */
void robot_turn_right();

/**
 * Set velocity of both motors to zero
 */
void robot_stop();

#endif //ARCTOS_ROBOT_ROBOT_H
