#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

typedef enum {speedChange, speedChangeAt, stopAt} motionType;

// info about a motion, some field are not useful for all type of motion
struct motionElement {
    motionType type;
    double speed;
    double cruiseSpeed;
    double distance;
    int finished;
    void (* callback)(struct motionElement*);
};

// type for callback functions, called when a motion is finished
// receives as argument a structure containing info about the action that has been finished
// callback should be like : void myCallback(struct motionElement actionJustFinished) {...}
typedef void (*motionCallback)(struct motionElement*);

// the fastest speed the robot is allowed to travel at (in m/s)
#define MAX_ALLOWED_SPEED 1

void queueSpeedChange(double speed, motionCallback onMotionFinished);
void queueSpeedChangeAt(double distance, double speed, motionCallback onFinished);
void queueStopAt(double distance, motionCallback onFinished);
void clearMotionQueue();

double getRobotDistance();
void setRobotDistance(double distance);

void setDistanceTolerance(double tolerance);
double getDistanceTolerance();
double getTargetSpeed();

void fastSpeedChange(double speed);

double computeTargetSpeed();

#endif
