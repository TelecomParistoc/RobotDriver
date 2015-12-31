#include "speedcontroller.h"
#include "controllerutils.h"
#include "queue.h"
#include <stdlib.h>
#include <math.h>

static double currentTargetSpeed = 0;

void queueSpeedChange(double speed, motionCallback onMotionFinished) {
    //TODO
}
void queueSpeedChangeAt(int distance, double speed, motionCallback onFinished) {
    //TODO
}
void queueStopAt(int distance, motionCallback onFinished){
    //TODO
}
void clearMotionQueue() {
    clearQueue();
}

double computeTargetSpeed() {
    //TODO
}
