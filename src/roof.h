#ifndef ROOF_H
#define ROOF_H

#ifdef __cplusplus
extern "C" {
#endif

void initRoof();

void setRedLed(int val);

void setGreenLed(int val);

void setYellowLed(int val);

int getStartJack();

#define BLUE_TEAM 0
#define YELLOW_TEAM 1
int getColor();

#define TEST_MODE 0
#define RUN_MODE 1
int getMode();

#ifdef __cplusplus
}
#endif

#endif
