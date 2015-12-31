#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

typedef enum {speedChange, speedChangeAt, stopAt} motionType;

// type for callback functions, called when a motion is finished
// receives as argument a structure containing info about the action that has been finished
// callback should be like : void myCallback(struct motionElement actionJustFinished) {...}
typedef void (*motionCallback)(struct motionElement);

// info about a motion, some field are not useful for all type of motion
struct motionElement {
    motionType type;
    double speed;
    int distance;
    motionCallback callback;
};

void queueSpeedChange(double speed, motionCallback onMotionFinished);
void queueSpeedChangeAt(int distance, double speed, motionCallback onFinished);
void queueStopAt(int distance, motionCallback onFinished);
void clearMotionQueue();

double computeTargetSpeed();

#endif
