#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H

#include "pathFinder.h"

#define LIN_SPEED     400 // mm/s
#define MAX_LIN_ACC   1000 // mm/s^2
#define RADIUS        30 // mm

/* Initialise motors with correct values
*/
void init();

/* Move from current position to next one
** startRadius: first radius of rotation
** dest: destination informations
*/
void goTo(int startRadius, move_t * dest);

#endif // MOVE_CONTROLLER_H
