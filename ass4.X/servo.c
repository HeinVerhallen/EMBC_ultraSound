#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "servo.h"
#include "timer.h"
#include "oc.h"
#include "pwm.h"

int chan = 0;

void servo_T2callback(){
    //set output high
    PORTAbits.RA15 = 1;
}

void servo_OC1callback(){
    //set output low
    PORTAbits.RA15 = 0;
}

unsigned char servo_init(int fpb, int channel, int angle){
    int tChannel = 2, oChannel = 1, period = 20;
    chan = channel;
    
    timer_register_T2callback(servo_T2callback);
    OC1_register_OC1callback(servo_OC1callback);
    
    //check if angle is a valid angle
    if(angle > 45 && angle < -45){
        angle = 0;
    }
    //initialize pwm signal
    pwm_init(fpb, tChannel, oChannel, period);
    
    servo_setpos(channel, angle);
    
    //setup the correct servo channel
            TRISAbits.TRISA15 = 0;
}

void servo_setpos(int channel, int angle){
    int tChannel = 2, oChannel = 1;
    
    //calculate width based on angle
    int width = servo_getWidth(angle);
    
    //update the pwm signal
    pwm_setWidth(tChannel, oChannel, width);
}

int servo_getWidth(int angle){
    float degree = 13.33333333;
    int neutral = 1500;
    int width = neutral + (int)((float)angle*degree);
    return width;
}