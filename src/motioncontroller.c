#include "motioncontroller.h"
#include "motordriver.h"
#include "headingcontroller.h"
#include "speedcontroller.h"
#include "controllerutils.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BLOCKING_ABS_THRESHOLD 40
#define BLOCKING_REL_THRESHOLD 0.95

int blockingHistoryFill = 0;
static void (*blockingCallback)(void) = NULL;
static void (*recalibrationCallback)(void) = NULL;

double maxAcceleration = 0.5; // in m.s^-2

void setMaxAcceleration(double acceleration) {
    if(acceleration != 0)
        maxAcceleration = acceleration;
}
double getMaxAcceleration() { return maxAcceleration; }

void setBlockingCallback(void (*callback)(void)) { blockingCallback = callback; }

void setRecalibrationCallback(void (*callback)(void)) { recalibrationCallback = callback; }


static void detectBlocking(double currentSpeed) {
    // keep the 20 last value
    static double distancesHistory[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static double speedsHistory[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static int headIndex=0;
    int i=0;
    double expected=0, real=0;
    distancesHistory[headIndex] = getRobotDistance();
    if(blockingHistoryFill > 29) {
        for(i=1;i<30;i++) {
            real += distancesHistory[(headIndex+i+1)%30]
                - distancesHistory[(headIndex+i)%30];
            expected += 1000*speedsHistory[(headIndex+i)%30]/getMotorDriverUpdateFreq();
        }
        if(fabs(expected - real) > 28)
            printf("absolute : %f\n", fabs(expected - real));
        //check the error is higher than the absolute threshold
        if(fabs(expected - real) > BLOCKING_ABS_THRESHOLD) {
            //check the relative error is higher than the relative threshold
            double relativeError = fabs(expected - real)
                /MAX(fabs(expected), fabs(real));
            printf("    relative : %f\n", relativeError);
            if(relativeError > BLOCKING_REL_THRESHOLD) {
                printf("        blocked !\n");
                //fastSpeedChange(0);
                //enableHeadingControl(0);
                if(blockingCallback != NULL)
                    blockingCallback();
                if(recalibrationCallback != NULL)
                    recalibrationCallback();
                // once a blocking has been detected, reset history to avoid flooding the user
                blockingHistoryFill=0;
            }
        }
    } else {
        blockingHistoryFill++;
    }
    speedsHistory[headIndex] = currentSpeed;
    headIndex = (headIndex+1)%30;
}

static void motionManager() {
    double differential = computeSpeedDifferential();
    double speed = computeTargetSpeed(getRobotDistance());

    if(getRotationDirection())
        differential = -differential;

    setRspeed(speed - differential);
    setLspeed(speed + differential);

    detectBlocking(speed);
}

int initMotionController() {
    int error = initMotorDriver();
    clearMotionQueue();
    setTargetHeading(getRobotHeading(), NULL);
    setMotorUpdateCallback(motionManager);
    // make sure the update timer is started
    motionManager();
    return error;
}
