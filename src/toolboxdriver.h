#ifndef TOOLBOXDRIVER_H
#define TOOLBOXDRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* initialize the toolbox driver, needs to be called before any other operations
 * returns -1 if it was unable to init the toolbox driver, 0 otherwise*/
int initToolboxDriver();

/* returns the current voltage of the battery powering motors and AX-12s (in volts) */
double getMotorPowerLevel();
/* returns the current voltage of the battery powering the sensors,
 * motioncontrollers and the raspberry pi (in volts) */
double getLogicPowerLevel();

/* get the state of a button (1 is logic high and 0 logic low). Buttons 1, 2 & 3
 * are connected to the toolbox board and buttons 4 & 5 are connected on raspberry
 * Pi's pins 13 and 15 */
int getButton(int number);
/* get the state of a sensor (1 is logic high and 0 logic low).
 * number : a sensor from 1 to 5 */
int getSensor(int number);
/* get the state of a collision detector. Returns 0 or 1. Depending on the model,
 * logical states can have different meanings
 * number : a collision detector from 1 to 5 */
int getCollisionDetector(int number);

/* set a function called when a sensor changes. It will be called whenever the
 * state of a sensor changes (from 0 to 1 or from 1 to 0), if the sensor callback
 * is enabled. (by default all the sensors triggers the callback) */
void setSensorsCallback(void (*callback)(void));
/* allow a sensor to trigger the callback when its state changes
 * number : a sensor from 1 to 5 */
void enableSensorCallback(int number);
/* prevent a sensor from triggering the callback when its state changes
 * number : a sensor from 1 to 5 */
void disableSensorCallback(int number);

/* set a function called when a collision detector changes. It will be called whenever the
 * state of a sensor changes (from 0 to 1 or from 1 to 0), if the sensor callback
 * is enabled. (by default all the sensors triggers the callback) */
void setCollisionsCallback(void (*callback)(void));
/* allow a collision detector to trigger the callback when its state changes
 * number : a collision detector from 1 to 5 */
void enableCollisionCallback(int number);
/* prevent a collision detector from triggering the callback when its state changes
 * number : a collision detector from 1 to 5 */
void disableCollisionCallback(int number);

/* set the duty cycle of a PWM output.
 *    number : a PWM output, from 1 to 4
 *    value : the duty cycle, from 0 (always low) to 255 (always high) */
void setPWM(int number, uint8_t value);
/* set the state of a LED
 *      number : a LED from 1 to 4
 *      state : 1 to turn on, 0 to turn off */
void setLED(int number, int state);

/* wait for the given amount of time (in milliseconds). Please note that actual
 * wait time may vary of +/- 10% the target value */
void waitFor(int milliseconds);


/* ====================== AX-12 ====================== */

/* Returns the position of the ax-12, */

int axGetPosition(int id);

/* Returns the state of the corresponding flag and clears it if it was set
 * for axHasFinishedMove, returns 2 if the destination wasn't reached
 * (probably beacuse the ax-12 was blocked)
 *
 * Useless in endless turn mode and concerns the las ax-12 used */

int axHasFinishedMove();
int axIsForcing();

/* High level function to control Ax-12 with id passed as parameter
 * Mustn't be called to modify settings of a moving ax-12
 * If you wan't to set speed or torque for a move, call this function
 * before the axMove function
 * If you don't want to change speed or torque, pass an invalid parameter
 * The last parameter enables you to chose the Ax-12 mode
 *
 * Torque : sets the maximum torque
 * Range  : 0 <= val <= 1023
 *
 * Speed  : sets the moving speed
 * Normal mode range :     0 <= val <= 1023
 * Wheel  mode range : -1023 <= val <= 1023
 *     a positive value is uses for a CounterClockWise rotation
 *     a negative value is used for a ClockWise rotation
 *
 * Mode : 0 for the Normal mode, 1 for Wheel moed (endless turn mode) */

void axSetTorqueSpeed(int id, int torque, int speed, int mode);

/* Moves Ax-12 with id passed as parameter to position (range 0 - 1023)
 * and calls callback() after the end of the move */

void axMove(int id, int position, void (* callback) (void));

void axStopPolling();

/* Calls the PIC routine that initializes all the Ax-12 */

void axReset();


#ifdef __cplusplus
}
#endif

#endif
