#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <robotdriver/movecontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/timing.h>
//#include "capteurs.h"

#define TAILLE_MAX 32
#define TAILLE_COORDONNEES 5
#define RADIUS 30
#define STARTRADIUS 30

int readAndCall(FILE * file, char c);

int main()
{
	// test translation
	int dist = 200;
	setGoalMeanDist(dist);
	while(abs(getDistReachedFromLastCommand() - dist) > 10)
		printf("%i\n", getDistReachedFromLastCommand());
	printf("[INFO] Dist reached ! \n");
	dist = 200;
	setGoalMeanDist(dist);
	while(abs(getDistReachedFromLastCommand() - dist) > 10)
		printf("%i\n", getDistReachedFromLastCommand());
	printf("[INFO] Dist reached ! \n");
	/*
	// test rotation
	setHeading(1800 * 576 / 360);
	int goalHeading = 0 * 576 / 360;
	setGoalHeading(goalHeading);
	while(abs(getHeading() - goalHeading) > 20)
		printf("heading: %u\n", getHeading());
	printf("[INFO] Goal heading reached\n");
	*//*
	int goalHeading = 4113;
	setGoalHeading(goalHeading);
	while(abs(getHeading() - goalHeading) > 20)
		printf("heading: %u\n", getHeading());
	printf("[INFO] Goal heading reached\n");
	*/
	return 0;
}
