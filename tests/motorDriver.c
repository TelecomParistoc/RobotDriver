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
  point_t dest_pos = {x + 20, y + 100};
  int dest_ang = heading + 900;
  printf("new position [%d,%d], new heading >%d°<\n", dest_pos.x, dest_pos.y, dest_ang/10);
  goForward(&dest_pos);
  rotate(dest_ang);
  printf("position [%d,%d], heading >%d°<\n", getPosX(), getPosY(), getHeading()/10);
	return 0;
}
