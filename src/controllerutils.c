#include "controllerutils.h"

extern double maxAcceleration;

inline double maxSpeedStep() {
    return maxAcceleration/getMotorDriverUpdateFreq();
}
double limitAcceleration(double lastSpeed, double targetSpeed) {
    if(targetSpeed-lastSpeed > maxSpeedStep())
        targetSpeed = lastSpeed + maxSpeedStep();
    else if(targetSpeed-lastSpeed < -maxSpeedStep())
        targetSpeed = lastSpeed - maxSpeedStep();
}
int getRobotDistance() {
    return (getRdistance()+getLdistance())/2;
}
