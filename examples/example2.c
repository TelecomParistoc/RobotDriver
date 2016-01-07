#include "robotdriver/motioncontroller.h"
#include "robotdriver/speedcontroller.h"
#include "robotdriver/headingcontroller.h"
#include <stdio.h>

/* This example shows how to go and turn : it will move of 300 mm, turn of 90°,
 * and move again of 300 mm*/

/* called when the robot reached its destination */
void endCallback(struct motionElement* element) {
 printf("I reached my destination !\n");
}

/* called when the robot completed its turn */
void endTurnCallback() {
    printf("I finished turning, now I will move of 300 mm again.\n");
    // move again of 300 mm
    setRobotDistance(0);
    queueSpeedChange(0.3, NULL);
    queueStopAt(300, endCallback);
}

/* this is called when the robot finished its first move */
void firstCallback(struct motionElement* element) {
    printf("I moved of 300 mm, now I will turn.\n");

    // turn of 90°, and call endTurnCallback when it's done
    turnOf(90, endTurnCallback);
}

int main() {
    // initialize the motion controller
    initMotionController();
    // reset robot distance
    setRobotDistance(0);

    // move of 300 mm
    queueSpeedChange(0.3, NULL); // if you don't need a callback, just use NULL
    queueStopAt(300, firstCallback);

    while(1);
    return 0;
}
