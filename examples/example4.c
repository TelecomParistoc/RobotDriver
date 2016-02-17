#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
#include <stdio.h>
#include <time.h>

/* In this example, we'll demonstrate how to use toolbox driver */

void delayMilli(int milliseconds) {
    struct timespec wait_time = {
        .tv_sec = 0,
        .tv_nsec = 1000000*milliseconds,
    };
    nanosleep(&wait_time, NULL);
}
static void collisionsCallback() {
    printf("coll3:%d, coll4:%d, coll5: %d\n", getCollisionDetector(3), getCollisionDetector(4), getCollisionDetector(5));
}
static void sensorsCallback() {
    printf("sensor1:%d, sensor2:%d, sensor3: %d, sensor4:%d, sensor5: %d\n", getSensor(1), getSensor(2), getSensor(3), getSensor(4), getSensor(5));
}

int main() {
    initToolboxDriver();
    initMotionController();

    setLED(1, 1);
    setLED(2, 0);
    setLED(3, 1);

    enableSensorCallback(1);
    disableSensorCallback(2);
    disableSensorCallback(3);
    disableSensorCallback(4);
    disableSensorCallback(5);
    setSensorsCallback(sensorsCallback);

    disableCollisionCallback(1);
    disableCollisionCallback(2);
    enableCollisionCallback(3);
    enableCollisionCallback(4);
    enableCollisionCallback(5);
    setCollisionsCallback(collisionsCallback);

    setAxActiveWheel(124);
    delayMilli(40);
    setAxSpeed(700);
    setAxPosition(150);
    while(1) {
        //delayMilli(00);
        //setAxPosition(30);
        //delayMilli(1000);
        //setAxPosition(40);
        //printf("%d\n", getAxPosition());
        //printf("%f\n", getMotorPowerLevel());
        //printf("bt1:%d, bt2:%d, bt3:%d, bt4:%d, bt5:%d\n", getButton(1), getButton(2), getButton(3), getButton(4), getButton(5));
        //printf("sensor1:%d, sensor2:%d, sensor3: %d, sensor4:%d, sensor5: %d\n", getSensor(1), getSensor(2), getSensor(3), getSensor(4), getSensor(5));
    }
    return 0;
}
