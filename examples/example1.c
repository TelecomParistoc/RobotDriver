#include "robotdriver/motioncontroller.h"
#include "robotdriver/speedcontroller.h"
#include "robotdriver/headingcontroller.h"
#include "robotdriver/motordriver.h"
#include <stdio.h>

/* This example shows how to move the robot of 400 mm in a straight line */

/* this is called when the robot reached its cruise speed */
void cruiseSpeedCallback(struct motionElement* element) {
    printf("I'm now travelling at 0.3 m/s !\n");
    printf("heading = %f\n", getRobotHeading());
    printf("distance = %f mm\n", getRobotDistance());
}

/* called when the robot reached its destination */
void endCallback(struct motionElement* element) {
    printf("I reached my destination !\n");
    printf("distance = %f mm\n", getRobotDistance());
}

int main() {
    // initialize the motion controller
    initMotionController();
    // reset robot distance
    setRobotDistance(0);

    // first increase the speed to 0.3 m/s, and call cruiseSpeedCallback when its done
    queueSpeedChange(0.3, cruiseSpeedCallback);
    // then stop when traveled for 400 mm, end call endCallback
    queueStopAt(400, endCallback);

    while(1);
    return 0;
}
