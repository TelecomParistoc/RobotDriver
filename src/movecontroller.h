#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H

// coordinates in mm
typedef struct point_s{
	int x;
	int y;
} point_t;

#define LIN_SPEED     400 // mm/s
#define MAX_LIN_ACC   1000 // mm/s^2
#define RADIUS        30 // mm
#define ANG_ACCURACY  15 // 0.1deg
#define DIST_ACCURACY 10 // mm

/* Initialise motors with correct values
*/
void init();

/* Move forward from current position to destination one
** dest:  destination absolute coodinates
*/
void goForward(point_t * dest);

/* Move backward from current position to destination one
** dest:  destination absolute coodinates
*/
void goBackward(point_t * dest);

/* Rotation to target angle
** angle: goal angle (in 0.1deg)
*/
void rotate(int angle);

#endif // MOVE_CONTROLLER_H
