#include "motioncontroller.h"
#include "motordriver.h"
#include "headingcontroller.h"
#include "speedcontroller.h"
#include "queue.h"
#include <stdlib.h>
#include <math.h>

#define BLOCKING_ABS_THRESHOLD 5
#define BLOCKING_REL_THRESHOLD 0.5

static int blockingHistoryFill = 0;
static void (*blockingCallback)(void) = NULL;

static double maxAcceleration = 0.5; // in m.s^-2
void setMaxAcceleration(double acceleration) {
    if(acceleration != 0)
        maxAcceleration = acceleration;
}
double getMaxAcceleration() { return maxAcceleration; }

void setBlockingCallback(void (*callback)(void)) { blockingCallback = callback; }

static detectBlocking() {
    // keep the 20 last value
    static int distancesHistory[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static double speedsHistory[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static int headIndex=0;
    int i=0, real=0;
    double expected=0;
    distancesHistory[headIndex] = getRobotDistance();
    if(blockingHistoryFill > 19) {
        for(i=1;i<20;i++) {
            real += distancesHistory[(headIndex+i+1)%20]
                - distancesHistory[(headIndex+i)%20];
            expected += distancesHistory[(headIndex+i)%20]/getMotorDriverUpdateFreq();
        }
        //check the error is higher than the absolute threshold
        if(abs(expected - real) > BLOCKING_ABS_THRESHOLD) {
            //check the relative error is higher than the relative threshold
            double relativeError = abs((expected - real)
                /((abs(expected) > abs(real)) ? expected : real));
            if(relativeError > BLOCKING_REL_THRESHOLD) {
                if(blockingCallback != NULL)
                    blockingCallback();
                // once a blocking has been detected, reset history to avoid flooding the user
                blockingHistoryFill=0;
            }
        }
    } else {
        blockingHistoryFill++;
    }
    speedsHistory[headIndex] = currentTargetSpeed;
    headIndex = (headIndex+1)%20;
}


static void motionManager() {
    double differential = computeSpeedDifferential();
    currentTargetSpeed = computeTargetSpeed();

    detectBlocking();

    setRspeed(currentTargetSpeed + differential);
    setLspeed(currentTargetSpeed - differential);
}

int initMotionController() {
    int error = initMotorDriver();
    clearMotionQueue();
    targetHeading = getRobotHeading();
    setMotorUpdateCallback(motionManager);
    // make sure the update timer is started
    motionManager();
    return error;
}
