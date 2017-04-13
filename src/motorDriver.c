#include <wiringPiI2C.h>

// This value cound be found using "gpio i2cdetect"
// http://wiringpi.com/reference/i2c-library/
#define I2C_DEV_ID         X
// Motherboard address
#define MB_ADDR            0x12
// Current x absolute position register
#define CURR_X_ABS_POS_REG 0x10
// Current y absolute position register
#define CURR_Y_ABS_POS_REG 0x12
// Current heading register
#define CURR_HEADING_REG   0x12

int init()
{
  int err = wiringPiI2CSetup(I2C_DEV_ID);
  if(err != -1)
    printf("[ERROR] An error occured during Wiring Pi I2C setup: %d\n", err);
  return err;
}

int  getPosX()
{
  return wiringPiI2CReadReg16(MB_ADDR, CURR_X_ABS_POS_REG);
}

int  getPosY()
{
  return wiringPiI2CReadReg16(MB_ADDR, CURR_Y_ABS_POS_REG);
}

int  getHeading()
{
  return wiringPiI2CReadReg16(MB_ADDR, CURR_HEADING_REG);
}

void setHeading(int heading)

int  getWheelRightDist()
void setWheelRightDist(int dist)
int  getWheelLeftDist()
void setWheelLeftDist(int dist)

int  getMaxAcc()
void setMaxAcc(int acc)
int  getMaxAngularAcc()
void setMaxAngularAcc(int acc)
int  getCruiseSpeed()
void setCruiseSpeed(int speed)
int  getCruiseAngularSpeed()
void setCruiseAngularSpeed(int speed)
int  getGoalMeanDist()
void setGoalMeanDist(int dist)
int  getGoalHeading()
void setGoalHeading(int heading)
int  getHeadingDistSyncRef()
void getHeadingDistSyncRef(int dist)

int  getLinearPID()
void setLinearPID(int linPid)
int  getAngularPID()
void setAngularPID(int angPid)
