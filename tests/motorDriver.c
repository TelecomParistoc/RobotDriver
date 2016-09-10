#include <wiringPiI2C.h>
#include "../src/motorDriver.h"
#include "../src/i2c-functions.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
  int res = I2Cread16(MB_ADDR, 0x01);
  printf(">%d-%d<\n", (res & 0xFF00) >> 8, res & 0x00FF);
  I2Cwrite16(MB_ADDR, 0x00, 0x0123);
  res = I2Cread16(MB_ADDR, 0x05);
  printf(">%d-%d<\n", (res & 0xFF00) >> 8, res & 0x00FF);
  res = I2Cread16(MB_ADDR, 0x00);
  printf(">%d-%d<\n", (res & 0xFF00) >> 8, res & 0x00FF);
  return 0;
}
