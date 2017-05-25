#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "movecontroller.h"
#include "motordriver.h"
#include "roof.h"
#include "timing.h"

#define DT1           (LIN_SPEED / MAX_LIN_ACC)
#define DEBUG         1

static int movingDir = DIR_NONE;

void init()
{
  setMaxAcc(MAX_LIN_ACC);
  setCruiseSpeed(LIN_SPEED);
}

int abs(int x)
{
  return(x >= 0 ? x : -x);
}

int getDirection() {
	return movingDir;
}

/*
** dist in mm
** sleep: in us
*/
void moveForward(int dist)
{
  if(dist > 0)
    movingDir = DIR_FORWARD;
  else if(dist < 0)
    movingDir = DIR_BACKWARD;

  setGoalMeanDist(dist);
  printf("goal dist: %d\n", dist);
  int currDist = getDistReachedFromLastCommand();
  while(abs(currDist - dist) > DIST_ACCURACY) {
    currDist = getDistReachedFromLastCommand();
    //printf("dist: %d %d\n", dist, currDist);
	waitFor(50); // ms
  }

  movingDir = DIR_NONE;
}

/*
** dist in mm
** sleep: in us
*/
void moveBackward(int dist)
{
  moveForward(-dist);
}

static int distance(int x1, int y1, point_t * p2)
{
  int dx = x1 - p2->x;
  int dy = y1 - p2->y;
  return sqrt(dx*dx + dy*dy);
}

/* Return heading (in 0.1deg) in order to be centered on target
** target: target position
*/
int getHeadingTo(point_t * target)
{
  // get current position
  int dx = target->x - getPosX();
  int dy = target->y - getPosY();
  printf("dx:%d dy:%d, atan:%f\n", dx, dy, atan2(dy,dx)*180/M_PI);
  // get angle from position to destination (in 0.1deg)
  if(atan2(dy, dx) < 0)
    return(atan2(dy, dx) * (1800 / M_PI) + 3600);
  return(atan2(dy, dx) * (1800 / M_PI));
}

/* Rotate robot
** angle: goal heading in 0.1deg
*/
void rotate(int angle) {
  if(getColor() == YELLOW_TEAM) {
    angle = 3600-angle;
  }

  printf("goalHeading: %d\n", angle);
  angle = angle * 576 / 360;
  printf("goalHeading: %u\n", angle);
  setGoalHeading(angle);
  // wait until new heading reached
  int heading = getHeading();
  int currHeadingDiff = abs(heading - angle);
  while(currHeadingDiff > ANG_ACCURACY && currHeadingDiff < 5760 - ANG_ACCURACY){
    heading = getHeading();
    //printf("heading: %u to %u\n", angle, heading);
    currHeadingDiff = abs(heading - angle);
	waitFor(50);
  }
}

void goForward(point_t * dest)
{
  printf("heading: %u, [%d,%d] to [%d,%d]\n", getHeading(), getPosX(), getPosY(), dest->x, dest->y);
  rotate(getHeadingTo(dest));
  moveForward(distance(getPosX(), getPosY(), dest));
}

void goBackward(point_t * dest)
{
#ifdef DEBUG
  printf("heading: %d, [%d,%d] to [%d,%d]\n", getHeading() / 10, getPosX(), getPosY(), dest->x, dest->y);
#endif
  int heading = getHeadingTo(dest) + 1800;
  if(heading >= 3600)
    heading -= 3600;
  rotate(heading);
  moveForward(distance(getPosX(), getPosY(), dest));
}
