#include "headingcontroller.h"
#include "motordriver.h"
#include "toolboxdriver.h"
#include "motioncontroller.h"
#include "controllerutils.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define MAX_HEADING_INTEGRAL 1000

static double maxDiffSpeed = 0.25;
static double targetHeading = 0;
static int headingControl = 1;
static double angularTolerance = 0.2; // in degrees
static void (*headingCallback)(void) = NULL;
static int turning = 0;
static double lastDifferential = 0;
static double integral = 0;

static double headingKp = 0.007;
static double headingKi = 0;
static double headingKd = 0.01;

void setHeadingTolerance(double tolerance) { angularTolerance = tolerance; }
double getHeadingTolerance() { return angularTolerance; }

void setMaxDiffSpeed(double diffspeed) { maxDiffSpeed = diffspeed; }
double getMaxDiffSpeed() { return maxDiffSpeed; }

void setHeadingKp(double coeff) { headingKp = coeff; }
double getHeadingKp() { return headingKp; }
void setHeadingKi(double coeff) { headingKi = coeff; }
double getHeadingKi() { return headingKi; }
void setHeadingKd(double coeff) { headingKd = coeff; }
double getHeadingKd() { return headingKd; }


double getTargetHeading() { return targetHeading; }
void setTargetHeading(double heading, void (*callback)(void)) {
    targetHeading = modulo(heading, 360);
    headingCallback = callback;
    turning = 1;
}

void turnOf(double turn, void (*callback)(void)) {
    setTargetHeading(modulo(targetHeading + turn, 360), callback);
}

void enableHeadingControl(int enable) {
    if(enable)
        headingControl = 1;
    else
        headingControl = 0;
}

static double computeTurn(double error) {
    double distanceError = error*WHEEL_DISTANCE*M_PI/360;
    double differential = sqrt(getMaxAcceleration()*fabs(distanceError)/1000.0)*SIGN(distanceError);

    if(fabs(differential) > limitAcceleration(fabs(lastDifferential),maxDiffSpeed))
        differential = limitAcceleration(lastDifferential, maxDiffSpeed*SIGN(distanceError));

    if(fabs(error) <= angularTolerance) {
        if(headingCallback != NULL) {
            scheduleIn(100, headingCallback);
            headingCallback = NULL;
        }
        integral = 0;
        turning = 0;
    }
    return differential;
}

static double computePID(double error) {
    static double lastError=0;
    double differential;

    integral += error;
    integral = clampValue(integral, MAX_HEADING_INTEGRAL);

    differential = headingKp*error + headingKi*integral + headingKd*(error - lastError);
    lastError = error;
    return differential;
}

double computeSpeedDifferential() {
    if(headingControl) {
        double differential;
        double error = targetHeading - getRobotHeading();
        if(error > 180)
            error -= 360;
        if(error < -180)
            error += 360;

        if(fabs(error) > 10)
            turning = 1;
        if(turning) {
            differential = computeTurn(error);
        } else {
            differential = computePID(error);
        }

        lastDifferential = differential;
        return differential;
    } else {
        integral = 0;
        lastDifferential = 0;
        return 0;
    }
}
