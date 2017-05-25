#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

// coordinates in mm
typedef struct point_s{
	int x;
	int y;
} point_t;

#define LIN_SPEED     400 // mm/s
#define MAX_LIN_ACC   1000 // mm/s^2
#define RADIUS        30 // mm
#define ANG_ACCURACY  20 // 0.1deg
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

/* returns the current moving direction */
#define DIR_NONE 0
#define DIR_FORWARD 1
#define DIR_BACKWARD 2
int getDirection();

// stop and resume move if an enemy is in our way
void stopRobot();
void resumeRobot();

#ifdef __cplusplus
}
#endif

#endif // MOVE_CONTROLLER_H
