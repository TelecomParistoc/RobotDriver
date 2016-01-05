#ifndef CONTROLLERUTILS
#define CONTROLLERUTILS

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SIGN(x) (((x) >= 0) ? 1 : -1)

double modulo(double value, int modulo);
double maxSpeedStep();
double limitAcceleration(double lastSpeed, double targetSpeed);

double clampValue(double value, double max);

#endif
