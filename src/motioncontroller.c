#include "motioncontroller.h"
#include "motordriver.h"
#include "headingcontroller.h"
#include "speedcontroller.h"
#include "controllerutils.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BLOCKING_ABS_THRESHOLD 20
#define BLOCKING_REL_THRESHOLD 0.95
#define BLOCKING_HISTORY_SIZE 30

int blockingHistoryFill = 0;
static void (*blockingCallback)(void) = NULL;
static void (*sideBlockingCallback)(void) = NULL;
static void (*recalibrationCallback)(void) = NULL;

double maxAcceleration = 0.5; // in m.s^-2

void setMaxAcceleration(double acceleration) {
    if(acceleration != 0)
        maxAcceleration = acceleration;
}
double getMaxAcceleration() { return maxAcceleration; }

void setBlockingCallback(void (*callback)(void)) { blockingCallback = callback; }
void setSideBlockingCallback(void (*callback)(void)) { sideBlockingCallback = callback; }
void setRecalibrationCallback(void (*callback)(void)) { recalibrationCallback = callback; }

static int isBlocked(double * distances, double * speeds, int head) {
    double real=0, expected=0;
    for(int i=1;i<BLOCKING_HISTORY_SIZE;i++) {
        real += distances[(head+i+1)%BLOCKING_HISTORY_SIZE] - distances[(head+i)%BLOCKING_HISTORY_SIZE];
        expected += 1000*speeds[(head+i)%BLOCKING_HISTORY_SIZE]/getMotorDriverUpdateFreq();
    }
    //if(fabs(expected - real) > 10)
    //    printf("absolute : %f\n", fabs(expected - real));
    //check the error is higher than the absolute threshold
    if(fabs(expected - real) > BLOCKING_ABS_THRESHOLD) {
        //check the relative error is higher than the relative threshold
        double relativeError = fabs(expected - real)/MAX(fabs(expected), fabs(real));
        printf("    relative : %f\n", relativeError);
        return relativeError > BLOCKING_REL_THRESHOLD;
    }
    return 0;
}
static void detectBlocking(double currentSpeedR, double currentSpeedL) {
    // keep the 30 last value
    static double distancesL[BLOCKING_HISTORY_SIZE] = {0};
    static double speedsL[BLOCKING_HISTORY_SIZE] = {0};
    static double distancesR[BLOCKING_HISTORY_SIZE] = {0};
    static double speedsR[BLOCKING_HISTORY_SIZE] = {0};
    static int headIndex=0;

    int blockedR=0, blockedL=0;
    distancesR[headIndex] = getRdistance();
    distancesL[headIndex] = getLdistance();
    if(blockingHistoryFill >= BLOCKING_HISTORY_SIZE) {
        blockedR = isBlocked(distancesR, speedsR, headIndex);
        blockedL = isBlocked(distancesL, speedsL, headIndex);
        if(blockedR)
            printf("        blockedR !\n");
        if(blockedL)
            printf("        blockedL !\n");
        if((blockedR && blockedL)&&sideBlockingCallback != NULL) {
            printf("            side blocked !\n");
            sideBlockingCallback();
        }
        if(blockedR && blockedL) {
            printf("            blocked !\n");
            if(blockingCallback != NULL)
                blockingCallback();
            if(recalibrationCallback != NULL)
                recalibrationCallback();
            // once a blocking has been detected, reset history to avoid flooding the user
            blockingHistoryFill=0;
        }
    } else {
        blockingHistoryFill++;
    }
    speedsR[headIndex] = currentSpeedR;
    speedsL[headIndex] = currentSpeedL;
    headIndex = (headIndex+1)%30;
}

static void motionManager() {
    double differential = computeSpeedDifferential();
    double speed = computeTargetSpeed(getRobotDistance());

    if(getRotationDirection())
        differential = -differential;

    setRspeed(speed - differential);
    setLspeed(speed + differential);

    detectBlocking(speed-differential, speed+differential);
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
