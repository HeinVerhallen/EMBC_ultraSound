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
#define RX_BUF_SIZE 64
#define minAngle -45
#define maxAngle 45
#define length 1
#define flashAngleOffset 45
#define waitTime CORE_FRQ/5

//function: GPIO_init();
// description: initializes the in-/outputs.
// pre:  none
// post: none
void GPIO_init();

//function: sendMessage(int angle);
// description: sends the current angle in the correct format to the uart.
// pre:  int angle - the currentangle
// post: none
void sendMessage(int angle);

//function: updateUart(int stringDone, int* angle);
// description: executes the correct instructions based on the uart input
// pre: int stringDone - boolean true if input command is fully complete 
//      int* angle - a pointer to the current angle
// post: none
void updateUart(int stringDone, int* angle);

//function: updateFlashMemory(char reading[length], int angle);
// description: update the SPIFLASH memory with the current angle
// pre: char reading[length] - variable which holds the currentangle with the memory offset
//      int angle - the current angle
// post: none
void updateFlashMemory(char reading[length], int angle);

void sendDistance(int distance);

void srf_Callback(unsigned short int dist);

#endif