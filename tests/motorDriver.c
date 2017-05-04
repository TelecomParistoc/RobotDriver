#include <wiringPiI2C.h>
#include "../src/moveController.h"
#include "../src/i2c-functions.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
  printf("maxAcc >%x<, cruiseSpeed >%x<\n", getMaxAcc(), getCruiseSpeed());
  init();
  printf("maxAcc >%x<, cruiseSpeed >%x<\n", getMaxAcc(), getCruiseSpeed());

  int x = getPosX();
  int y = getPosY();
  int heading = getHeading();
  printf("position [%x,%x], heading >%x<\n", x, y, heading);
  move_t dest = {x + 20, y + 100, heading + 900, RADIUS};
  printf("new position [%x,%x], new heading >%x<\n", dest.x, dest.y, dest.heading);
  goTo(&dest);
  printf("position [%x,%x], heading >%x<\n", getPosX(), getPosY(), getHeading());
	return 0;
}
