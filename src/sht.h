#ifndef SHT_H_
#define SHT_H_

#include "lpc_types.h"

typedef struct s_sht1x {
	uint8_t port;
	uint8_t sclk;
	uint8_t data;
	uint8_t pinsel_port;
	uint8_t pinsel_sclk;
	uint8_t pinsel_data;
	float temperature;
	float humidity;
	float dewpoint;
} t_sht1x;

void sht1x_init(t_sht1x *s);


#endif /* SHT_H_ */
