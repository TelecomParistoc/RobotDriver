#include "controllerutils.h"

extern double maxAcceleration;

inline double maxSpeedStep() {
    return maxAcceleration/getMotorDriverUpdateFreq();
}
double limitAcceleration(double lastSpeed, double targetSpeed) {
    if(targetSpeed-lastSpeed > maxSpeedStep())
        return lastSpeed + maxSpeedStep();
    else if(targetSpeed-lastSpeed < -maxSpeedStep())
        return lastSpeed - maxSpeedStep();
    else
        return targetSpeed;
}
int getRobotDistance() {
    return (getRdistance()+getLdistance())/2;
}

double clampValue(double value, double max) {
    value = value > max ? max : value;
    value = value < -max ? -max : value;
}
