#include "headingcontroller.h"
#include "motordriver.h"
#include "motioncontroller.h"
#include "controllerutils.h"
#include <stdlib.h>

#define MAX_HEADING_INTEGRAL 4000
#define MAX_DIFF_SPEED 0.5

static double targetHeading = 0;
static double currentHeading = 0;
static int headingControl = 1;
static double angularTolerance = 0.5; // in degrees
static void (*headingCallback)(void) = NULL;

static double headingKp = 0.002;
static double headingKi = 0.0001;
static double headingKd = 0.001;

void setHeadingTolerance(double tolerance) { angularTolerance = tolerance; }
double getHeadingTolerance() { return angularTolerance; }

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
    setTargetHeading((targetHeading + turn + 360) % 360, callback);
}

void enableHeadingControl(int enable) {
    if(enable)
        headingControl = 1;
    else
        headingControl = 0;
}

static void filterCurrentHeading() {
    static double headings[] = {0,0,0};
    int i=0;
    double median;
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
        integral += error;
        integral = clampValue(integral, MAX_HEADING_INTEGRAL);
        differential = headingKp*error + headingKi*integral + headingKd*(error - lastError);
        differential = clampValue(limitAcceleration(lastDifferential, differential), MAX_DIFF_SPEED);
        lastDifferential = differential;
        lastError = error;

        if(abs(error) <= angularTolerance && headingCallback != NULL) {
            headingCallback();
            headingCallback = NULL;
        }
        return differential;
    } else
        return 0;
}
