#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
#include <stdio.h>
#include <stdlib.h>

/* In this example, we'll demonstrate how to use toolbox driver */

static void first() {
	printf("1 second\Än");
}
static void second() {
        printf("2 second\n");
}
static void third() {
        printf("3 second\n");
}
static void fourth() {
        printf("4 second\n");
}
int main() {
	initMotionController();
	initToolboxDriver();

	scheduleIn(1000, first);
	scheduleIn(2000, second);
	scheduleIn(3000, third);
	scheduleIn(4000, fourth);

	while(1);
	return 0;
}
