#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "ax12actions.h"
#include "robotdriver/driver.h"

static char locks = 0;

void end_of_move()
{
	locks--;
}

void wait()
{
	while(locks != 0);
}
/*
int abs(int n){
	if(n < 0)
		return -n;
	return n;
}
*/
void move_to(int id, float angle)
{
	locks++;
	int ret = AX12move(id, angle, &end_of_move);
	while(ret != 0){
		printf("ret %d\n", ret);
		printf("while in move\n");
		ret = AX12move(id, angle, &end_of_move);
	}
}

void move_to_block(int id, float angle)
{
	while(1){
		move_to(id, angle);
		wait();
		if(abs(AX12getPosition(id) - angle) < 10)
			break;
		printf("[ERROR] Angle shifting: %f != %f\n", angle, AX12getPosition(id));
	}
}

