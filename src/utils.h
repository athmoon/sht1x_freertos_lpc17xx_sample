#ifndef UTILS_H_
#define UTILS_H_


#define P015     15
#define TXD1     15
#define SCK0     15
#define SCK      15
#define P027     27
#define SDA0     27
#define USB_SDA  27


#define D_OUT    1
#define D_IN	 0

#define MODE_GPIO	0


void pinMode(int pin, int mode);
void pinDirection(int pin, int dir);


#endif /* UTILS_H_ */
