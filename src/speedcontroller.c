#include "speedcontroller.h"
#include "controllerutils.h"
#include "queue.h"
#include <stdlib.h>
#include <math.h>

static double currentTargetSpeed = 0;

static void queueAction(motionType type, double speed, double distance, motionCallback callback) {
    struct motionElement* action = malloc(sizeof(struct motionElement));

    action->type = type;
    action->speed = clampValue(speed, MAX_ALLOWED_SPEED);
    action->cruiseSpeed = -23;
    action->distance = distance;
    action->finished = 0;
    action->callback = callback;
    addToQueue(action);

}
void queueSpeedChange(double speed, motionCallback onMotionFinished) {
    queueAction(speedChange, speed, 0, onMotionFinished);
}
void queueSpeedChangeAt(int distance, double speed, motionCallback onFinished) {
    queueAction(speedChangeAt, speed, distance, onFinished);
}
void queueStopAt(int distance, motionCallback onFinished){
    queueAction(stopAt, 0, distance, onFinished);
}
void clearMotionQueue() {
    clearQueue();
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
static double computeStopAt(struct motionElement* action) {
    if(action->cruiseSpeed=-23)
        action->cruiseSpeed = currentTargetSpeed;

}
double computeTargetSpeed() {

}
