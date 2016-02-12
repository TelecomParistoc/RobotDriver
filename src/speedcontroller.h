/*                        #### SPEED CONTROLLER ####
 * This file provides a high level control of the robot speed, allowing it to
 * move of a given distance and to change it's speed along the way.
 *
 * The controller is based on a queue, allowing the programmer to load complex
 * moves at once. There are 3 different queueable actions :
 * - speedChange : reach target speed as soon as possible (but without exceeding
 *      maximum acceleration)
 * - speedChangeAt : change the speed so that the target speed is
 *      reached at a given distance (accerate or slow down before).
 * - stopAt : stop the robot at a given distance : behaves like speedChangeAt with
 *      target speed = 0, but maintains distance control after completion (if
 *      the robot is pushed afterwards, it will return to its target location)
 *
 * for more details, see queue* methods
 *
 * ! WARNING ! : to use the following methods, you need to call initMotionController()
 * first */

#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* the different queueable actions */
typedef enum {speedChange, speedChangeAt, stopAt} motionType;

/* info about a motion, some field are not useful for all type of motion */
struct motionElement {
    motionType type;
    double speed;       // target speed
    double cruiseSpeed; // speed before speed change for speedChangeAt and stopAt
    double distance;    // target distance
    int finished;       // 1 if the action is finished, 0 otherwise
    void (* callback)(struct motionElement*); // a function called when the action is done
};

/* type for callback functions, called when an action is done
 * receives as argument a structure containing info about the action that has been finished
 * callback should be like : void myCallback(struct motionElement* actionJustFinished) {...} */
typedef void (*motionCallback)(struct motionElement*);

/* the fastest speed the robot is allowed to travel at (in m/s) (rebuild library
 * to change) */
#define MAX_ALLOWED_SPEED 1


/* add a speedChange action to the queue : when executed, will change speed to
 * the given value as soon as possible without exceeding maximum acceleration.
 * arguments :
 *      - speed : the target speed in m/s
 *      - onMotionFinished : a function called when target speed is reached or NULL
 *          the given function will receive in argument a struct motionElement
 *          containing info about this action */
void queueSpeedChange(double speed, motionCallback onMotionFinished);

/* add a speedChangeAt action to the queue : when executed, will change speed to
 * the given value when the target distance is reached. The speed will be
 * maintained its previous value as long as possible, then changed so that to
 * target speed is reached at the given distance.
 * arguments :
 *      - distance : the distance where the new speed should be reached (in mm)
 *      - speed : the target speed in m/s
 *      - onMotionFinished : a function called when target speed is reached or NULL
 *          the given function will receive in argument a struct motionElement
 *          containing info about this action */
void queueSpeedChangeAt(double distance, double speed, motionCallback onFinished);

/* add a stopAt action to the queue : when executed, will stop the robot at the
 * given distance. As long as a new action is not added to the queue, a distance
 * control remains active, keeping the robot at its location.
 * arguments :
 *      - distance : the distance where the robot should stop (in mm)
 *      - onMotionFinished : a function called when target speed is reached or NULL
 *          the given function will receive in argument a struct motionElement
 *          containing info about this action */
void queueStopAt(double distance, motionCallback onFinished);

/* remove all actions from the queue (speed will be kept constant) */
void clearMotionQueue();


/* get/set robot distance (in mm). This is actually the mean of the 2 wheels distance.
 * IMPORTANT : make sure the distance DO NOT EXCEED ~ +/- 12000, otherwise PIC
 * counters may otherflow, producing erratic behavior. Distance should therefore
 * be often reset */
double getRobotDistance();
void setRobotDistance(double distance);

/* get/set the maximum distance error (in mm). For speedChangeAt and stopAt :
 * when the robot distance is within target distance +/- distance tolerance, the
 * controller calls the callback (if not NULL) and the next action (if any) is
 * loaded. Default is 1 mm. */
void setDistanceTolerance(double tolerance);
double getDistanceTolerance();

/* returns current target speed in m/s. This not the actual speed (see
 * getRspeed()/getLspeed() in motordriver.h for that). */
double getTargetSpeed();

 /* change speed ignoring queue and maximum acceleration : useful for emergency
  * it will clear the queue and set the speed to the given value (in m/s) */
void fastSpeedChange(double speed);



// DO NOT CALL, used by the implementation
double computeTargetSpeed(double distance);

#ifdef __cplusplus
}
#endif

#endif
