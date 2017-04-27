#include "moveController.h"

#define DT1           (LIN_SPEED / MAX_LIN_ACC)

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

typedef struct move_s {
  int x; // in mm
	int y; // in mm
	int angle; // in 0.1 deg
	int radius; // in mm
} move_t;

void goTo(move_t * dest)
{
  // get current position
  int currX = getPosX();
  int currY = getPosY();
  // find path to destination
  move_t depart = {currX, currY, getHeading(), RADIUS};
  interpoints_t interpoints = computeInterpoints(&depart, dest);
  // moving by following the path
  rotate(&path->rot1);
  int dt2 = path->rot1.angle * path->rot1.radius / (MAX_LIN_ACC * DT1) - DT1;
  //gotForward(path->forward, dt2);
  //rotate(&path->rot2);
}
