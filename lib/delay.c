#include "delay.h"

void ms(u32 ms)
{
	STK_LOAD = 8999;
	STK_VAL  = 8999;
	STK_CTRL = 0;
	STK_CTRL = 1 << 0;

	while (ms)
	{
		while (!(STK_CTRL & 1 << 16));
		ms--;
	}
	return;
}

void us(u32 us)
{
	STK_LOAD = us * 8;
	STK_VAL  = us * 8;
	STK_CTRL = 0;
	STK_CTRL = 1 << 0;

	while (!(STK_CTRL & 1 << 16));

	return;
}
