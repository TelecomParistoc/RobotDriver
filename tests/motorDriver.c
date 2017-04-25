#include <wiringPiI2C.h>
#include "../src/motorDriver.h"
#include "../src/i2c-functions.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
  int res = getHeading();
  printf(">%x<\n", res);
  	//setHeading(1000);
	for(int i = 0; i < 100000; i++)
{
	__asm__ __volatile__ ("nop");
}
	printf("new heading: >%x<\n", getHeading() & 0xffff);
	return 0;
}
