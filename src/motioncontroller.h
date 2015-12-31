/*                 #### MOTION CONTROLLER ####
 * This file provides high level functions to move the robot.
 * Moves are divided in two types : robot's speed change and heading change (turn)
 * for speed change, see speedcontroller.h,
 * for heading change, see headingcontroller.h.
 * BUT FIRST, the motion control engine must be initialized with initMotionController()
 * This controller also provides blocking detection : when the robot should be
 * moving but is not, a callback is called.*/

#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

/* this function must be called before any motion */
int initMotionController();

/* get/set robot's maximum acceleration : any speed change or heading change will
 * be slowed down to make sure the robot won't accerate more than the given
 * acceleration value (in m.s^-2). Default value is 0.5 m.s^-2 */
void setMaxAcceleration(double acceleration);
double getMaxAcceleration();

/* set a function to be called when the robot is blocked
 * the function should be like : void myFunction() {...}
 * if the robot stays blocked, the function is called about every 20*(update period)
 * by default, it is called about every 200ms
 * (for more info, see getMotorDriverUpdateFreq() in motordriver.h) */
void setBlockingCallback(void (*callback)(void));

#endif
