/*                        #### SPEED CONTROLLER ####
 * This file provides a high level control of the robot heading.
 *
 * This controller uses a PID controller with acceleration limitation to set and
 * maintain robot's heading.
 *
 * ! WARNING ! : to use the following methods, you need to call initMotionController()
 * first */

#ifndef HEADINGCONTROLLER_H
#define HEADINGCONTROLLER_H

// distance between coding wheels in mm (depends of the robot)
#define WHEEL_DISTANCE 190

/* get current target heading (in degrees, 0-360, clockwise). The target
 * heading is maintained as long as the controller is enabled. */
double getTargetHeading();
/* set robot's heading (will cause the robot to turn until is reaches the desired
 * heading, to correct current heading value, see setRobotHeading() in
 * motordriver.h).
 * arguments :
 *      - heading : the target heading in degrees (0-360, clockwise)
 *      - finishedCallback : a function to be called when the robot reaches the
 *          desired heading. The function should be like : void myCallback() {...} */
void setTargetHeading(double heading, void (*finishedCallback)(void));
/* rotate the robot of the given angle.
 * arguments :
 *      - turn : the angle in degrees (0-360, clockwise)
 *      - finishedCallback : a function to be called when the robot reaches the
 *          desired heading. The function should be like : void myCallback() {...} */
void turnOf(double turn, void (*finishedCallback)(void));

/* get/set the maximum differential between right and left wheel speed when
 * turning (in m/s) */
void setMaxDiffSpeed(double diffspeed);
double getMaxDiffSpeed();

/* get/set the maximum heading error (in degrees). When the robot heading is
 * within target heading +/- heading tolerance, the controller calls the callback
 * specified in setTargetHeading or turnOf (if not NULL). The callback is called
 * only once, the first time the robot enters the valid zone, but control of the
 * heading is maintained afterwards. Default is 1 deg. */
void setHeadingTolerance(double tolerance);
double getHeadingTolerance();

/* disable heading control when enable=0, enable when enable=1
 * Default is enabled (enable=1) */
void enableHeadingControl(int enable);

/* get/set PID coefficients. Please note that when error is too high, integral
 * control is disabled to prevent oscillations */
void setHeadingKp(double coeff);
double getHeadingKp();
void setHeadingKi(double coeff);
double getHeadingKi();
void setHeadingKd(double coeff);
double getHeadingKd();


// DO NOT CALL, used by the implementation
double computeSpeedDifferential();

#endif
