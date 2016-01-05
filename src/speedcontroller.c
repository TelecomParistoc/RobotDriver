#include "speedcontroller.h"
#include "controllerutils.h"
#include "motioncontroller.h"
#include "motordriver.h"
#include "queue.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static double currentTargetSpeed = 0;
static double distanceTolerance = 1; // in mm
extern int blockingHistoryFill;

void setDistanceTolerance(double tolerance) { distanceTolerance = fabs(tolerance); }
double getDistanceTolerance() { return distanceTolerance; }

static void queueAction(motionType type, double speed, double distance, motionCallback callback) {
    struct motionElement* action = malloc(sizeof(struct motionElement));
    action->type = type;
    action->speed = clampValue(speed, MAX_ALLOWED_SPEED);
    action->cruiseSpeed = -23;
    action->distance = distance;
    action->finished = 0;
    action->callback = callback;

    if(getHead() != NULL) {
        struct motionElement* head = getHead();
        if(head->finished)
            removeHead();
    }
    addToQueue(action);
}
void queueSpeedChange(double speed, motionCallback onMotionFinished) {
    queueAction(speedChange, speed, 0, onMotionFinished);
}
void queueSpeedChangeAt(double distance, double speed, motionCallback onFinished) {
    queueAction(speedChangeAt, speed, distance, onFinished);
}
void queueStopAt(double distance, motionCallback onFinished){
    queueAction(stopAt, 0, distance, onFinished);
}
void clearMotionQueue() {
    clearQueue();
}
void fastSpeedChange(double speed) {
    clearMotionQueue();
    currentTargetSpeed = speed;
}

double getTargetSpeed() {
    return currentTargetSpeed;
}
double getRobotDistance() {
    double dist = (getRdistance()+getLdistance())/2;
    if(dist > 11000)
        printf("!! WARNING !! : distance may overflow, please reset it ASAP\n");
    return dist;
}
void setRobotDistance(double distance) {
    double Roffset = getRdistance() - getRobotDistance();
    double Loffset = getLdistance() - getRobotDistance();
    // clear blocking detector history to avoid false positive
    blockingHistoryFill = 0;
    setRdistance(Roffset + distance);
    setLdistance(Loffset + distance);
}

static void terminateMotionAction(struct motionElement* action) {
    if(action->callback != NULL)
        action->callback(action);
    action->finished = 1;
}
static double computeSpeedChange(struct motionElement* action) {
    double speed = limitAcceleration(currentTargetSpeed, action->speed);
    if(speed == action->speed) {
        terminateMotionAction(action);
        removeHead();
    }
    return speed;
}
static double computeSpeedWithDistanceTarget(struct motionElement* action) {
    double speed;
    double distanceError = action->distance - getRobotDistance();
    // before the slow down phase, keep the speed to the previous value
    if(action->cruiseSpeed == -23)
        action->cruiseSpeed = currentTargetSpeed;

    speed = sqrt(2*getMaxAcceleration()*fabs(distanceError)/1000.0 + pow(action->speed,2))*SIGN(distanceError);

    if(fabs(speed) > fabs(action->cruiseSpeed))
        speed = fabs(action->cruiseSpeed)*SIGN(distanceError);

    // detect when target distance is reached for the first time
    if(fabs(distanceError) <= distanceTolerance) {
        // make sure the speed is exactly the desired speed
        speed = action->speed;
        if(action->finished == 0)
            terminateMotionAction(action);
    }
    return speed;
}
static double computeStopAt(struct motionElement* action) {
    double speed = computeSpeedWithDistanceTarget(action);
    // remove the finished action only if there's other queued actions
    if(action->finished && getQueueSize() > 1)
        removeHead();
    return speed;
}
static double computeSpeedChangeAt(struct motionElement* action) {
    double speed = computeSpeedWithDistanceTarget(action);
    // remove the action
    if(action->finished) {
        removeHead();
    }
    return speed;
}

double computeTargetSpeed() {
    struct motionElement* action = getHead();
    if(action != NULL) {
        switch (action->type) {
            case speedChange:
                currentTargetSpeed = computeSpeedChange(action);
                break;
            case speedChangeAt:
                currentTargetSpeed = computeSpeedChangeAt(action);
                break;
            case stopAt:
                currentTargetSpeed = computeStopAt(action);
                break;
        }
    }
    return currentTargetSpeed;
}
