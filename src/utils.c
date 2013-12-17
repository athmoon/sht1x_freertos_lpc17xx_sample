#include "utils.h"

#include "LPC17xx.h"

void pinMode(int pin, int mode)
{
	if (pin == P015 && mode == MODE_GPIO)
	{
		LPC_PINCON->PINSEL0	&= (~(0b11 << 30));
	}
}


void pinDirection(int pin, int dir)
{
	LPC_GPIO0->FIODIR |= (1 << 22);
}
