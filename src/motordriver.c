// http://wiringpi.com/reference/i2c-library/
#include <wiringPiI2C.h>
#include <stdio.h>
#include "motordriver.h"
#include "timing.h"
#include "i2c-functions.h"

// Sleep time during two commands in ms
#define SLEEP                 10

// This value cound be found using "gpio i2cdetect"
#define I2C_DEV_ID            0x20
// Current x absolute position register (high weight)
#define CURR_X_ABS_POS_H_REG  0x82
// Current x absolute position register (low weight)
#define CURR_X_ABS_POS_L_REG  0x80
// Current y absolute position register (high weight)
#define CURR_Y_ABS_POS_H_REG  0x86
// Current y absolute position register (low weight)
#define CURR_Y_ABS_POS_L_REG  0x84
// Current heading register
#define CURR_HEADING_REG      0x90
// Current right wheel distance register (high weight)
#define CURR_R_WHEEL_DIST_H_REG 0x8A
// Current right wheel distance register (low weight)
#define CURR_R_WHEEL_DIST_L_REG 0x88
// Current left wheel distance register (high weight)
#define CURR_L_WHEEL_DIST_H_REG 0x8E
// Current left wheel distance register (low weight)
#define CURR_L_WHEEL_DIST_L_REG 0x8C
// Maximum linear acceleration register
#define MAX_ACC_REG           0x06
// Maximum linear angular acceleration register
#define MAX_ACC_ANG_REG       0x08
// Cruise speed linear register
#define CRUISE_SPEED_REG      0x0A
// Cruise speed angular register
#define CRUISE_ANG_SPEED_REG  0x0A
// Goal minimum distance register
#define GOAL_MIN_DIST_REG     0xA0
// Goal heading register
#define GOAL_HEADING_REG      0xA2
// Heading distance sync reference register
#define HEADING_DIST_SYNC_REF 0xA4
// Linear p coefficiant register
#define LINEAR_P_REG          0x0E
// Linear i coefficiant register
#define LINEAR_I_REG          0x10
// Linear d coefficiant register
#define LINEAR_D_REG          0x12
// Angular p coefficiant register
#define ANGULAR_P_REG         0x14
// Angular i coefficiant register
#define ANGULAR_I_REG         0x16
// Angular d coefficiant register
#define ANGULAR_D_REG         0x18
// Last traveled distance high register
#define LAST_TRAVELED_H_REG   0x94
// Last traveled distance low register
#define LAST_TRAVELED_L_REG   0x92

void setPosX(int x) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_X_ABS_POS_H_REG, x / 65536);
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_X_ABS_POS_L_REG, x % 65536);
}
void setPosY(int y) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_Y_ABS_POS_H_REG, y / 65536);
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_Y_ABS_POS_L_REG, y % 65536);
}
int  getPosX() {
  waitFor(SLEEP);
  int res = I2Cread16(MB_ADDR, CURR_X_ABS_POS_H_REG);
  if(res == -1) return -1;
  waitFor(SLEEP);
  return res * 65536 + I2Cread16(MB_ADDR, CURR_X_ABS_POS_L_REG);
}
int  getPosY() {
  waitFor(SLEEP);
  int res = I2Cread16(MB_ADDR, CURR_Y_ABS_POS_H_REG);
  if(res == -1) return -1;
  waitFor(SLEEP);
  return res * 65536 + I2Cread16(MB_ADDR, CURR_Y_ABS_POS_L_REG);
}
int  getHeading() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, CURR_HEADING_REG);
}
void setHeading(int heading) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_HEADING_REG, heading);
}

int  getWheelRightDist() {
  waitFor(SLEEP);
  int res = I2Cread16(MB_ADDR, CURR_R_WHEEL_DIST_H_REG);
  if(res == -1) return -1;
  waitFor(SLEEP);
  return res * 65536 + I2Cread16(MB_ADDR, CURR_R_WHEEL_DIST_L_REG);
}
void setWheelRightDist(int dist) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_R_WHEEL_DIST_H_REG, dist / 65536);
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_R_WHEEL_DIST_L_REG, dist % 65536);
}
int  getWheelLeftDist() {
  waitFor(SLEEP);
  int res = I2Cread16(MB_ADDR, CURR_L_WHEEL_DIST_H_REG);
  if(res == -1) return -1;
  waitFor(SLEEP);
  return res * 65536 + I2Cread16(MB_ADDR, CURR_L_WHEEL_DIST_L_REG);
}
void setWheelLeftDist(int dist) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_L_WHEEL_DIST_H_REG, dist / 65536);
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CURR_L_WHEEL_DIST_L_REG, dist % 65536);
}

int  getMaxAcc() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, MAX_ACC_REG);
}
void setMaxAcc(int acc) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, MAX_ACC_REG, acc);
}
int  getMaxAngularAcc() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, MAX_ACC_ANG_REG);
}
void setMaxAngularAcc(int acc) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, MAX_ACC_ANG_REG, acc);
}
int  getCruiseSpeed() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, CRUISE_SPEED_REG);
}
void setCruiseSpeed(int speed) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CRUISE_SPEED_REG, speed);
}
int  getCruiseAngularSpeed() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, CRUISE_ANG_SPEED_REG);
}
void setCruiseAngularSpeed(int speed) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, CRUISE_ANG_SPEED_REG, speed);
}
void setGoalMeanDist(int dist) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, GOAL_MIN_DIST_REG, dist);
}
void setGoalHeading(int heading) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, GOAL_HEADING_REG, heading);
}
void setHeadingDistSyncRef(int dist) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, HEADING_DIST_SYNC_REF, dist);
}

int  getLinearP() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, LINEAR_P_REG);
}
void setLinearP(int linP) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, LINEAR_P_REG, linP);
}
int  getLinearI() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, LINEAR_I_REG);
}
void setLinearI(int linI) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, LINEAR_I_REG, linI);
}
int  getLinearD() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, LINEAR_D_REG);
}
void setLinearD(int linD) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, LINEAR_D_REG, linD);
}
int  getAngularP() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, ANGULAR_P_REG);
}
void setAngularP(int angP) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, ANGULAR_P_REG, angP);
}
int  getAngularI() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, ANGULAR_I_REG);
}
void setAngularI(int angI) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, ANGULAR_I_REG, angI);
}
int  getAngularD() {
  waitFor(SLEEP);
  return I2Cread16(MB_ADDR, ANGULAR_D_REG);
}
void setAngularD(int angD) {
  waitFor(SLEEP);
  I2Cwrite16(MB_ADDR, ANGULAR_D_REG, angD);
}

int getDistReachedFromLastCommand() {
  waitFor(SLEEP);
  int res = I2Cread16(MB_ADDR, LAST_TRAVELED_H_REG) ;
  if(res == -1) return -1;
  waitFor(SLEEP);
  return res * 65536 + I2Cread16(MB_ADDR, LAST_TRAVELED_L_REG);
}
