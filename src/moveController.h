#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H

#include "pathFinder.h"

#define LIN_SPEED     0.4 // m/s
#define MAX_LIN_ACC   1.0 // m/s^2
#define RADIUS        30 // mm

/* Initialise motors with correct values
*/
void init();
/* Move from current position to next one
** dest: destination informations
*/
void goTo(move_t * dest);

#endif // MOVE_CONTROLLER_H
