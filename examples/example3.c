#include "robotdriver/motioncontroller.h"
#include "robotdriver/speedcontroller.h"
#include "robotdriver/motordriver.h"
#include <stdio.h>

/* In this example,  we'll recalibrate the robot against a wall, that is :
 *      - go toward a wall
 *      - slow down near the wall while disabling heading control
 *      - detect when the robot is blocking against the wall, reset the distance
 *          and heading and go backward
 * This process aims to compensate small errors with distance and heading */

/* when the robot approaches the wall, disable the heading control to let the
 * wall correct our heading */
void disableHeadingControl(struct motionElement* element) {
    printf("I'm approaching the wall : disabling heading control.\n");
    enableHeadingControl(0);
}

/* called if the robot didn't block against the wall */
void noWallCallback(struct motionElement* element) {
    printf("I didn't find any wall !\n");
}

/* called when the robot reached its destination */
void endCallback(struct motionElement* element) {
    printf("I reached my destination !\n");
}

/* called when a blocking is detected */
void onRobotBlocked() {
    // stop the robot and cancel pending moves
    fastSpeedChange(0);
    // set the heading to the correct value and re-enable heading control
    setRobotHeading(0);
    enableHeadingControl(1);
    // reset distance
    setRobotDistance(0);

    // go backward
    queueSpeedChange(0.2, NULL);
    queueStopAt(200, endCallback);
}

int main() {
    // initialize the motion controller
    initMotionController();
    // reset robot distance
    setRobotDistance(0);

    // set the function called when the robot blocks
    setBlockingCallback(onRobotBlocked);

    // let's say there's a wall at a distance of about 300-350 mm
    queueSpeedChange(0.3, NULL); // go toward the wall
    queueSpeedChange(300, 0.1, disableHeadingControl); // slow down when approaching
    queueStopAt(400, noWallCallback); // if there was no wall, stop

    while(1);
    return 0;
}
