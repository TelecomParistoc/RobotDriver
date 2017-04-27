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

int goTo(int x, int y)
{
  // get current position
  int currX = getPosX();
  int currY = getPosY();
  // find path to destination
  path_t * path = malloc(sizeof(path_t));
  if(pathFromAToB(path, currX, currY, x, y))
  {
    printf("[ERROR] An error occured during path finding\n");
    return 1;
  }
  // moving by following the path
  rotate(path->rot1);
  int dt2 = path->rot1->angle * path->rot1->radius / (MAX_LIN_ACC * DT1) - DT1;
  gotForward(path->forward, dt2);
  rotate(path->rot2);
  return 0;
}
