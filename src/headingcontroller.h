#ifndef HEADINGCONTROLLER_H
#define HEADINGCONTROLLER_H

double getTargetHeading();
void setTargetHeading(double heading, void (*finishedCallback)(void));
void turnOf(double turn, void (*finishedCallback)(void));

double getCurrentHeading();
void setHeadingTolerance(double tolerance);
double getHeadingTolerance();
void enableHeadingControl(int enable);

void setHeadingKp(double coeff);
double getHeadingKp();
void setHeadingKi(double coeff);
double getHeadingKi();
void setHeadingKd(double coeff);
double getHeadingKd();

double computeSpeedDifferential();

#endif
