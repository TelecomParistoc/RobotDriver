#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
#include <stdio.h>
#include <time.h>
#include <wiringPi.h>

/* In this example, we'll test the "tapis roulant" (i don't know how it translates in English and I'm too lazy to search)*/

void delayMilli(int milliseconds) {
	struct timespec wait_time = {
		.tv_sec = 0,
		.tv_nsec = 1000000
	};
	for(int i=0; i<milliseconds; i++)
		nanosleep(&wait_time, NULL);
}

int main() {
	initToolboxDriver();
	initMotionController();

	axSetTorqueSpeed(133, -1, 50, 0);
	//axMove(144, 768, NULL);
	delayMilli(100);
	
	printf("%d\n", axGetPosition(133));
	while(1) {
		printf("%d\n", axGetPosition(133));
        	delayMilli(100);
	}

	return 0;
}
