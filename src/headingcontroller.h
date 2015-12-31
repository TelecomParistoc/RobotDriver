#ifndef HEADINGCONTROLLER_H
#define HEADINGCONTROLLER_H

double getTargetHeading();
void setTargetHeading(double heading, void (*finishedCallback)(void));
void turnOf(double turn, void (*finishedCallback)(void));

void setHeadingTolerance(double tolerance);
double getHeadingTolerance();
void enableHeadingControl(int enable);

double computeSpeedDifferential();

#endif
