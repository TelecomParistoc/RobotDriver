#ifndef PATH_FINDER_H
#define PATH_FINDER_H

typedef struct point_s{
	float x;
	float y;
} point_t;

typedef struct move_s {
  int x; // in mm
	int y; // in mm
	int angle; // in 0.1 deg
	int radius; // in mm
} move_t;

typedef struct interpoints_s {
	point_t tan1;
	point_t tan2;
	float alpha1;
	float alpha2;
} interpoints_t;

typedef struct rotation_s {
  int radius; // in mm
  int angle; // in radian
} rotation_t;

typedef struct path_s {
  rotation_t rot1;
  int forward;
  rotation_t rot2;
} path_t;

/* Get the path in order to go from a point A to another (B) and save the result into "path"
** A: departure informations
** B: destination informations
*/
interpoints_t computeInterpoints(move_t *depMove, move_t *destMove);

#endif // PATH_FINDER_H
