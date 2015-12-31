#include "headingcontroller.h"
#include "motordriver.h"
#include "motioncontroller.h"
#include "controllerutils.h"
#include <stdlib.h>

static double targetHeading = 0;
static int headingControl = 1;
static double angularTolerance = 0.5; // in degrees
static void (*headingCallback)(void) = NULL;

void setHeadingTolerance(double tolerance) { angularTolerance = tolerance; }
double getHeadingTolerance() { return angularTolerance; }

double getTargetHeading() { return targetHeading; }
void setTargetHeading(double heading, void (*callback)(void)) {
    targetHeading = heading;
    headingCallback = callback;
}
void turnOf(double turn, void (*callback)(void)) {
    setTargetHeading((targetHeading + turn + 360) % 360, callback);
}

void enableHeadingControl(int enable) {
    if(enable)
        headingControl = 1;
    else
        headingControl = 0;
}

double computeSpeedDifferential() {
    if(headingControl) {
        static double lastDifferential = 0;
        static double integral = 0;
        double differential = 0;

        //TODO

        differential = limitAcceleration(lastDifferential, differential);
        lastDifferential = differential;
        return differential;
    } else
        return 0;
}
