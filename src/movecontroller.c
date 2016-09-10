#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "movecontroller.h"
#include "motordriver.h"

#define DT1           (LIN_SPEED / MAX_LIN_ACC)
#define DEBUG         1

void init()
{
  setMaxAcc(MAX_LIN_ACC);
  setCruiseSpeed(LIN_SPEED);
}

int abs(int x)
{
  return(x >= 0 ? x : -x);
}

/*
** dist in mm
** sleep: in us
*/
void moveForward(int dist)
{
  setGoalMeanDist(dist);
  printf("goal dist: %d\n", dist);
  int currDist = getDistReachedFromLastCommand();
  while(abs(currDist - dist) > DIST_ACCURACY) {
    currDist = getDistReachedFromLastCommand();
    //printf("dist: %d %d\n", dist, currDist);
  }
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
  int currX = getPosX();
  int currY = getPosY();
  // get angle from position to destination (in 0.1deg)
  return atan2(target->y - currY, target->x - currX) * (1800 / M_PI);
}

/* Rotate robot
** angle: goal heading in 0.1deg
*/
void rotate(int angle)
{
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