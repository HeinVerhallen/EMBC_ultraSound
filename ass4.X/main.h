#ifndef _TESTSERVO_H
#define _TESTSERVO_H

/**
 * Platform: Basys MX3
 * Authors: Hein Verhallen, Youri Tils
 * 
 * Description
 */
#define BTNC    PORTFbits.RF0
#define BTNR    PORTBbits.RB8
#define SPIFLASH_PROG_ADDR  0x100

void GPIO_init();

void sendMessage(int angle);

void sendDistance();

void srf_Callback(unsigned short int dist);

#endif