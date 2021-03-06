#ifndef MOTOR_DEFINE_H
#define MOTOR_DEFINE_H

// Motherboard address
#define MB_ADDR               0x12

int  getPosX();
int  getPosY();
void setPosX(int x);
void setPosY(int y);
int  getHeading();
void setHeading(int heading);

int  getWheelRightDist();
void setWheelRightDist(int dist);
int  getWheelLeftDist();
void setWheelLeftDist(int dist);

int  getMaxAcc();
void setMaxAcc(int acc);
int  getMaxAngularAcc();
void setMaxAngularAcc(int acc);
int  getCruiseSpeed();
void setCruiseSpeed(int speed);
int  getCruiseAngularSpeed();
void setCruiseAngularSpeed(int speed);
void setGoalMeanDist(int dist);
void setGoalHeading(int heading);
void setHeadingDistSyncRef(int dist);

int  getLinearP();
void setLinearP(int linP);
int  getLinearI();
void setLinearI(int linI);
int  getLinearP();
void setLinearP(int linD);
int  getAngularP();
void setAngularP(int angP);
int  getAngularI();
void setAngularI(int angI);
int  getAngularD();
void setAngularD(int angD);

int  getDistReachedFromLastCommand();

void emergencyStop();
void emergencyResume();

#endif // MOTOR_DEFINE_H
