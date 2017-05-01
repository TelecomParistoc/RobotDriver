#include <wiringPiI2C.h>
#include "../src/moveController.h"
#include "../src/motorDriver.h"
#include "../src/i2c-functions.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
  printf("maxAcc >%d<, cruiseSpeed >%d<\n", getMaxAcc(), getCruiseSpeed());
  init();
  printf("maxAcc >%d<, cruiseSpeed >%d<\n", getMaxAcc(), getCruiseSpeed());

  int x = getPosX();
  int y = getPosY();
  int heading = getHeading();
  printf("position [%d,%d], heading >%d°<\n", x, y, heading/10);
  move_t dest = {x + 20, y + 100, heading + 900, RADIUS};
  printf("new position [%d,%d], new heading >%d°<\n", dest.x, dest.y, dest.angle/10);
  goTo(RADIUS, &dest);
  printf("position [%d,%d], heading >%d°<\n", getPosX(), getPosY(), getHeading()/10);
	return 0;
}
