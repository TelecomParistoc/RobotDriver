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

void rotate(rotation_t * rot)
{
  setMaxAngularAcc(MAX_LIN_ACC / rot->radius);
  setCruiseAngularSpeed(MAX_LIN_ACC * DT1 / rot->radius);
  int goalHeading = getHeading() + rot->direction * rot->angle;
  setGoalHeading(goalHeading);
  setGoalMeanDist(rot->radius * rot->angle);
  usleep(DT1 * 1000000);
}

/*
** dist in mm
** sleep: in us
*/
void goForward(int dist, int sleep)
{
  setGoalMeanDist(dist);
  usleep(sleep);
}

int scalVect(point_t * a, point_t * b)
{
  return a->x * b->x + a->y * b->y;
}

int distance(point_t * p1, point_t * p2)
{
  int dx = p1->x - p2->x;
  int dy = p1->y - p2->y;
  return sqrt(dx*dx + dy*dy);
}

int getDirection(int heading, point_t * pos, point_t * dest)
{
  point_t dir = {sin(heading * M_PI / 1800)*100, -cos(heading * M_PI / 1800)*100};
  point_t dir_dest = {dest->x - pos->x, dest->y - pos->y};
  return((scalVect(&dir, &dir_dest) > 0) ? 1 : -1);
}

void goTo(move_t * dest)
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
  move_t depart = {currX, currY, heading, RADIUS};
  interpoints_t interpoints = computeInterpoints(&depart, dest);
  // moving by following the path
  // make first rotaion
  rotation_t rot1 = {RADIUS, getDirection(heading, &pos, &interpoints.tan1), interpoints.alpha1 - heading};
#ifdef DEBUG
  printf("rot1: %d, %d, %d\n", rot1.radius, rot1.direction, rot1.angle / 10);
#endif
  rotate(&rot1);
  int dt2 = rot1.angle * rot1.radius / (MAX_LIN_ACC * DT1) - DT1;
  int dist = distance(&interpoints.tan1, &interpoints.tan2);
#ifdef DEBUG
  printf("dist: %d\n", dist);
#endif
  goForward(dist, dt2);
  rotation_t rot2 = {RADIUS+1, getDirection(interpoints.alpha2, &interpoints.tan2, &pos_dest), dest->angle - heading};
#ifdef DEBUG
  printf("rot2: %d, %d, %d\n", rot1.radius, rot1.direction, rot1.angle / 10);
#endif
  rotate(&rot2);
}
