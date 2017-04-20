#include <wiringPiI2C.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
  init();
  int res = wiringPiI2CReadReg16(MB_ADDR, 0x00);
  printf("%c%c", (res & 0xFF00) >> 8, res & 0x00FF);
  return 0;
}
