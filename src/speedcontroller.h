#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

typedef enum {speedChange, speedChangeAt, stopAt} motionType;

// type for callback functions, called when a motion is finished
// receives as argument a structure containing info about the action that has been finished
// callback should be like : void myCallback(struct motionElement actionJustFinished) {...}
typedef void (*motionCallback)(struct motionElement*);

// info about a motion, some field are not useful for all type of motion
struct motionElement {
    motionType type;
    double speed;
    double cruiseSpeed;
    int distance;
    int finished;
    motionCallback callback;
};

// the fastest speed the robot is allowed to travel at (in m/s)
#define MAX_ALLOWED_SPEED 1

void queueSpeedChange(double speed, motionCallback onMotionFinished);
void queueSpeedChangeAt(int distance, double speed, motionCallback onFinished);
void queueStopAt(int distance, motionCallback onFinished);
void clearMotionQueue();

void setDistanceTolerance(unsigned int tolerance);
unsigned int getDistanceTolerance();

double computeTargetSpeed();

#endif
