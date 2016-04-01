#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
#include <stdio.h>
#include <time.h>
#include <wiringPi.h>

/* In this example, we'll demonstrate how to use toolbox driver */

int pos = 0;

static void collisionsCallback() {
	printf("coll3:%d, coll4:%d, coll5: %d\n", getCollisionDetector(3), getCollisionDetector(4), getCollisionDetector(5));
}
static void sensorsCallback() {
	printf("sensor1:%d, sensor2:%d, sensor3: %d, sensor4:%d, sensor5: %d\n", getSensor(1), getSensor(2), getSensor(3), getSensor(4), getSensor(5));
}

static void axCallback() {
	printf("AxEndMoveFlag = %d\n", axHasFinishedMove());
	if(pos)
		pos = 0;
	else
		pos = 1000;
	axMove(124, pos, axCallback);
}

int main() {
	initToolboxDriver();
	initMotionController();

	// turn on some LEDs on the roof
	setLED(1, 1);
	setLED(2, 0);
	setLED(3, 1);

	// enable callback only for sensors in use and call sensorsCallback when something happens
	enableSensorCallback(1);
	disableSensorCallback(2);
	disableSensorCallback(3);
	disableSensorCallback(4);
	disableSensorCallback(5);
	setSensorsCallback(sensorsCallback);

	// enable callback only for collision detectors in use and call collisionsCallback when something happens
	disableCollisionCallback(1);
	disableCollisionCallback(2);
	enableCollisionCallback(3);
	enableCollisionCallback(4);
	enableCollisionCallback(5);
	setCollisionsCallback(collisionsCallback);

	// move an AX12 and call axCallback when it's done
	axSetTorqueSpeed(124, 1023, 200, 0);
	axMove(124, pos, axCallback);

	while(1);
	return 0;
}
