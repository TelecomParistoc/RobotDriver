#include "headingcontroller.h"
#include "motordriver.h"
#include "motioncontroller.h"
#include "controllerutils.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define MAX_HEADING_INTEGRAL 1000

static double maxDiffSpeed = 0.25;
static double targetHeading = 0;
static double currentHeading = 0;
static int headingControl = 1;
static double angularTolerance = 0.5; // in degrees
static void (*headingCallback)(void) = NULL;

static double headingKp = 0.005;
static double headingKi = 0.0002;
static double headingKd = 0;

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
    targetHeading = heading;
    headingCallback = callback;
}

double getCurrentHeading() { return currentHeading; }

void turnOf(double turn, void (*callback)(void)) {
    setTargetHeading(modulo(targetHeading + turn, 360), callback);
}

void enableHeadingControl(int enable) {
    if(enable)
        headingControl = 1;
    else
        headingControl = 0;
}

static void filterCurrentHeading() {
    static double headings[] = {0,0,0};
    headings[0] = getRobotHeading();
    // abort in case of communication error
    if(headings[0] < 0)
        return;
    // median filter the 3 last headings
    if(headings[0] > MIN(headings[1], headings[2]) && headings[0] <= MAX(headings[1], headings[2])) {
        currentHeading = headings[0];
    } else if(headings[1] > MIN(headings[0], headings[2]) && headings[1] <= MAX(headings[0], headings[2])) {
        currentHeading = headings[1];
    } else {
        currentHeading = headings[2];
    }
    headings[1] = headings[0];
    headings[2] = headings[1];
}
double computeSpeedDifferential() {
    if(headingControl) {
        static double integral=0, lastError=0, lastDifferential=0;
        double differential, error;
        filterCurrentHeading();

        error = targetHeading-currentHeading;
        if(error > 180)
            error -= 360;
        if(error < -180)
            error += 360;
        if(fabs(error)<5)
            integral += error;
        else
            integral = 0;
        integral = clampValue(integral, MAX_HEADING_INTEGRAL);
        differential = headingKp*error + headingKi*integral + headingKd*(error - lastError);
        differential = clampValue(limitAcceleration(lastDifferential, differential), maxDiffSpeed);
        lastDifferential = differential;
        lastError = error;

        if(fabs(error) <= angularTolerance && headingCallback != NULL) {
            headingCallback();
            headingCallback = NULL;
        }
        return differential;
    } else
        return 0;
}
