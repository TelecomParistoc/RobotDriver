// http://wiringpi.com/reference/i2c-library/
#include <wiringPiI2C.h>
#include <stdio.h>
#include "motorDriver.h"

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

int init() {
  int err = wiringPiI2CSetup(I2C_DEV_ID);
  if(err != -1)
    printf("[ERROR] An error occured during Wiring Pi I2C setup: %d\n", err);
  return err;
}
int  getPosX() {
  int res = wiringPiI2CReadReg16(MB_ADDR, CURR_X_ABS_POS_H_REG);
  return res * 65536 + wiringPiI2CReadReg16(MB_ADDR, CURR_X_ABS_POS_L_REG);
}
int  getPosY() {
  int res = wiringPiI2CReadReg16(MB_ADDR, CURR_Y_ABS_POS_H_REG);
  return res * 65536 + wiringPiI2CReadReg16(MB_ADDR, CURR_Y_ABS_POS_L_REG);
}
int  getHeading() {
  return wiringPiI2CReadReg16(MB_ADDR, CURR_HEADING_REG);
}
void setHeading(int heading) {
  wiringPiI2CWriteReg16(MB_ADDR, CURR_HEADING_REG, heading);
}

int  getWheelRightDist() {
  int res = wiringPiI2CReadReg16(MB_ADDR, CURR_R_WHEEL_DIST_H_REG);
  return res * 65536 + wiringPiI2CReadReg16(MB_ADDR, CURR_R_WHEEL_DIST_L_REG);
}
void setWheelRightDist(int dist) {
  wiringPiI2CWriteReg16(MB_ADDR, CURR_R_WHEEL_DIST_H_REG, dist / 65536);
  wiringPiI2CWriteReg16(MB_ADDR, CURR_R_WHEEL_DIST_L_REG, dist % 65536);
}
int  getWheelLeftDist() {
  int res = wiringPiI2CReadReg16(MB_ADDR, CURR_L_WHEEL_DIST_H_REG);
  return res * 65536 + wiringPiI2CReadReg16(MB_ADDR, CURR_L_WHEEL_DIST_L_REG);
}
void setWheelLeftDist(int dist) {
  wiringPiI2CWriteReg16(MB_ADDR, CURR_L_WHEEL_DIST_H_REG, dist / 65536);
  wiringPiI2CWriteReg16(MB_ADDR, CURR_L_WHEEL_DIST_L_REG, dist % 65536);
}

int  getMaxAcc() {
  return wiringPiI2CReadReg16(MB_ADDR, MAX_ACC_REG);
}
void setMaxAcc(int acc) {
  wiringPiI2CWriteReg16(MB_ADDR, MAX_ACC_REG, acc);
}
int  getMaxAngularAcc() {
  return wiringPiI2CReadReg16(MB_ADDR, MAX_ACC_ANG_REG);
}
void setMaxAngularAcc(int acc) {
  wiringPiI2CWriteReg16(MB_ADDR, MAX_ACC_ANG_REG, acc);
}
int  getCruiseSpeed() {
  return wiringPiI2CReadReg16(MB_ADDR, CRUISE_SPEED_REG);
}
void setCruiseSpeed(int speed) {
  wiringPiI2CWriteReg16(MB_ADDR, CRUISE_SPEED_REG, speed);
}
int  getCruiseAngularSpeed() {
  return wiringPiI2CReadReg16(MB_ADDR, CRUISE_ANG_SPEED_REG);
}
void setCruiseAngularSpeed(int speed) {
  wiringPiI2CWriteReg16(MB_ADDR, CRUISE_ANG_SPEED_REG, speed);
}
void setGoalMeanDist(int dist) {
  wiringPiI2CWriteReg16(MB_ADDR, GOAL_MIN_DIST_REG, dist);
}
void setGoalHeading(int heading) {
  wiringPiI2CWriteReg16(MB_ADDR, GOAL_HEADING_REG, heading);
}
void setHeadingDistSyncRef(int dist) {
  wiringPiI2CWriteReg16(MB_ADDR, HEADING_DIST_SYNC_REF, dist);
}

int  getLinearP() {
  return wiringPiI2CReadReg16(MB_ADDR, LINEAR_P_REG);
}
void setLinearP(int linP) {
  wiringPiI2CWriteReg16(MB_ADDR, LINEAR_P_REG, linP);
}
int  getLinearI() {
  return wiringPiI2CReadReg16(MB_ADDR, LINEAR_I_REG);
}
void setLinearI(int linI) {
  wiringPiI2CWriteReg16(MB_ADDR, LINEAR_I_REG, linI);
}
int  getLinearD() {
  return wiringPiI2CReadReg16(MB_ADDR, LINEAR_D_REG);
}
void setLinearD(int linD) {
  wiringPiI2CWriteReg16(MB_ADDR, LINEAR_D_REG, linD);
}
int  getAngularP() {
  return wiringPiI2CReadReg16(MB_ADDR, ANGULAR_P_REG);
}
void setAngularP(int angP) {
  wiringPiI2CWriteReg16(MB_ADDR, ANGULAR_P_REG, angP);
}
int  getAngularI() {
  return wiringPiI2CReadReg16(MB_ADDR, ANGULAR_I_REG);
}
void setAngularI(int angI) {
  wiringPiI2CWriteReg16(MB_ADDR, ANGULAR_I_REG, angI);
}
int  getAngularD() {
  return wiringPiI2CReadReg16(MB_ADDR, ANGULAR_D_REG);
}
void setAngularD(int angD) {
  wiringPiI2CWriteReg16(MB_ADDR, ANGULAR_D_REG, angD);
}
