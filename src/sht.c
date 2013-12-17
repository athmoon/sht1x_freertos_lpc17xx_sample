#include "sht.h"

#include "LPC17xx.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

// P0.27 - SDA0  ////  P0.15 - SCK0

void sht1x_init(t_sht1x *s)
{
	GPIO_SetDir(s->port, (1 << s->data) | (1 << s->sclk), 1);	// set both to output
	GPIO_ClearValue(s->port, (1 << s->data) | (1 << s->sclk));	// write 0
	PINSEL_CFG_Type pcfg;
	pcfg.Funcnum = PINSEL_FUNC_0;
	pcfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	pcfg.Pinmode = PINSEL_PINMODE_PULLUP;
	pcfg.Pinnum = s->pinsel_data;
	pcfg.Portnum = s->pinsel_port;
	PINSEL_ConfigPin(&pcfg);		//make sure pullup is on
	GPIO_SetDir(s->port, (1 << s->data), 0);	// data to input mode
	s->temperature = s->humidity = s->dewpoint = 0.0f;

}


char sht1x_writebyte(t_sht1x *s, uint8_t value)
{

}

uint8_t sht1x_readbyte(t_sht1x *s, Bool send_ack )
{

}
