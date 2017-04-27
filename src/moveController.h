#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H

#include "pathFinder.h"

#define LIN_SPEED     0.4 // m/s
#define MAX_LIN_ACC   1.0 // m/s^2

/* Initialise motors with correct values
*/
void init();
/* Move from current position to (x,y) position
** x: x coordinate of destination (in mm)
** y: y coordinate of destination (in mm)
*/
int  goTo(int x, int y);

#endif // MOVE_CONTROLLER_H
