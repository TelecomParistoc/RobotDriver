#ifndef PATH_FINDER_H
#define PATH_FINDER_H

typedef struct rotation_s {
  int radius; // in mm
  int direction; // -1:left, 1:right
  int angle; // in radian
} rotation_t;

typedef struct path_s {
  rotation_t * rot1;
  int forward;
  rotation_t * rot2;
} path_t;

/* Get the path in order to go from a point A to another (B) and save the result into "path"
** path: output path
** xA:   x coordinate of starting point
** yA:   y coordinate of starting point
** xB:   x coordinate of arrival point
** yB:   y coordinate of arrival point
*/
int pathFromAToB(path_t * path, int xA, int yA, int xB, int yB);

#endif // PATH_FINDER_H
