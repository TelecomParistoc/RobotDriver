#include "../src/motordriver.h"
#include <stdio.h>

static int mockIsInitialized = 0;
static int mockDistance = 0;
static double mockSpeedL = 0.0;
static double mockSpeedR = 0.0;
static double mockHeading = 0.0;
static void (*updCallback)() = NULL;

int initMotorDriver() {
    mockIsInitialized = 1;
    return 0;
}
int assertInitialized() { return mockIsInitialized; }

int getLdistance() {
    return mockDistance;
}
int getRdistance() {
    return mockDistance;
}
void setLdistance(int distance) {
    mockDistance = distance;
}
void setRdistance(int distance) {
    mockDistance = distance;
}
double getLspeed() {
    return mockSpeedL;
}
double getRspeed() {
    return mockSpeedR;
}
void setLspeed(double speed) {
    printf("L speed is %f\n", speed);
    mockSpeedL = speed;
}
void setRspeed(double speed) {
    printf("R speed is %f\n", speed);
    mockSpeedR = speed;
}

uint8_t getKp() { return 1; }
uint8_t getKi() { return 1; }
uint8_t getKd() { return 1; }

void setKp(uint8_t coeff) {}
void setKi(uint8_t coeff) {}
void setKd(uint8_t coeff) {}

void setMotorUpdateCallback(void (*callback)()) {
        updCallback = callback;
}
void nextStep() {
    if(updCallback != NULL)
        updCallback();
}

double getRobotHeading() { return mockHeading; }
void setRobotHeading(double heading) { mockHeading = heading; }

void setMotorDriverUpdateFreq(int max_freq) {}
int getMotorDriverUpdateFreq() { return 100; }
