#ifndef TOOLBOXDRIVER_H
#define TOOLBOXDRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* initialize the toolbox driver, needs to be called before any other operations
 * returns -1 if it was unable to init the toolbox driver, 0 otherwise*/
int initToolboxDriver();

double getMotorPowerLevel();
double getLogicPowerLevel();
int getButton(int number);
int getSensor(int number);
int getCollisionDetector(int number);

int getAxPosition();

void setSensorsCallback(void (*callback)(void));
void enableSensorCallback(int number);
void disableSensorCallback(int number);

void setCollisionsCallback(void (*callback)(void));
void enableCollisionCallback(int number);
void disableCollisionCallback(int number);

void setPWM(int number, uint8_t value);

void setLED(int number, int state);

void setAxActiveWheel(uint8_t id);
void setAxActiveDefault(uint8_t id);
void setAxSpeed(int speed);
void setAxPosition(int position);
void setAxTorque(int torque);

#ifdef __cplusplus
}
#endif

#endif
