#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "moveController.h"
#include "motorDriver.h"

#define DT1           (LIN_SPEED / MAX_LIN_ACC)
#define DEBUG         1

void init()
{
  setMaxAcc(MAX_LIN_ACC);
  setCruiseSpeed(LIN_SPEED);
}

void rotate(rotation_t * rot, int currHeading)
{
  int goalHeading = rot->angle;
  setGoalHeading(goalHeading);
  int dist = rot->radius * (rot->angle - currHeading);
  setGoalMeanDist(dist);
  while(getDistReachedFromLastCommand() < dist);
}

/*
** dist in mm
** sleep: in us
*/
void goForward(int dist)
{
  setGoalMeanDist(dist);
  while(getDistReachedFromLastCommand() < dist);
}

int distance(point_t * p1, point_t * p2)
{
  int dx = p1->x - p2->x;
  int dy = p1->y - p2->y;
  return sqrt(dx*dx + dy*dy);
}

void goTo(int startRadius, move_t * dest)
{
  // get current position
  int currX = getPosX();
  int currY = getPosY();

  point_t pos = {currX, currY};
  point_t pos_dest = {dest->x, dest->y};
  int heading = getHeading();
#ifdef DEBUG
  printf("heading: %d, [%d,%d] to [%d,%d]\n", heading / 10, currX, currY, dest->x, dest->y);
#endif
  // find path to destination
  move_t depart = {currX, currY, heading, startRadius};
  interpoints_t interpoints = computeInterpoints(&depart, dest);
  // moving by following the path
  // make first rotaion
  rotation_t rot1 = {startRadius, interpoints.alpha1};
#ifdef DEBUG
  printf("rot1: %d, %d\n", rot1.radius, rot1.angle / 10);
#endif
  rotate(&rot1, heading);
  int dist = distance(&interpoints.tan1, &interpoints.tan2);
#ifdef DEBUG
  printf("dist: %d\n", dist);
#endif
  goForward(dist);
  rotation_t rot2 = {dist.radius, dest->angle};
#ifdef DEBUG
  printf("rot2: %d, %d\n", rot2.radius, rot2.angle / 10);
#endif
  rotate(&rot2, heading);
}
