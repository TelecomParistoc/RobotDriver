#include <wiringPiI2C.h>
#include "../src/moveController.h"
#include "../src/motorDriver.h"
#include "../src/i2c-functions.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
  init();

  int x = getPosX();
  int y = getPosY();
  int heading = getHeading();
  printf("position [%d,%d], heading >%d°<\n", x, y, heading/10;
  return 0;
}
