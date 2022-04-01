#include <xc.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include <stdio.h>
#include "i2c.h"
#include "timer.h"
#include "srf.h"

#define TIMER_CHANNEL 3

SrfDistanceCallback srfDistanceCallback;
unsigned char srfDistanceCallbackFlag = 0;

void srf_registerDistanceCallback(SrfDistanceCallback ptr_srfDistanceCallback) {
    srfDistanceCallback = ptr_srfDistanceCallback;
    srfDistanceCallbackFlag = 1;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) Timer3ISR(void) {
    unsigned short int distance;
    srf_getDistance(&distance);
    
    if (srfDistanceCallbackFlag) {
        srfDistanceCallback(distance);
    }
    
    timer_detachInterrupt(TIMER_CHANNEL);
    
    IFS0bits.T3IF = 0;
}

void srf_init(int freq) {
    I2C_Init(freq);
    timer_init(freq, TIMER_CHANNEL, 100, 0, 0);
}

void srf_range(){
    if (srf_startRanging()) {
        timer_reset(TIMER_CHANNEL);
        timer_attachInterrupt(TIMER_CHANNEL, 4);
    }
}
 
unsigned char srf_startRanging() {
    unsigned char data1[2] = { 0x02, 0x8c };
    unsigned char data2[2] = { 0x01, 0x02 };
    unsigned char data3[2] = { 0x00, 0x51 };
 
    I2C_Write(0x70, data1, 2, 0);
    I2C_Write(0x70, data2, 2, 0);
    I2C_Write(0x70, data3, 2, 0);
    
    return 1;
}
 
unsigned char srf_getDistance(unsigned short int *distance) {
    unsigned char data[1] = { 0x02 };
    unsigned char res[2] = { 0, 0 };
 
    I2C_Write(0x70, data, 1, 0);
    I2C_Read(0x70, res, 2);
 
    *distance = res[0] * 256 + res[1];
    
    return 1;
}