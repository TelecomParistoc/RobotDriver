#ifndef CONTROLLERUTILS
#define CONTROLLERUTILS

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SIGN(x) (((x) >= 0) ? 1 : -1)

inline double maxSpeedStep();
double limitAcceleration(double lastSpeed, double targetSpeed);
int getRobotDistance();

double clampValue(double value, double max);

#endif
