#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "main.h"
#include "pragma.h"
#include "shell.h"
#include "servo.h"
#include "spiflash.h"
#include "srf.h"

#define RX_BUF_SIZE 64

int distance = 0;

void main(){
    //initialization
    int angle = 0, pAngle = 0, rState = 0, rPState = 0, cState = 0, cPState = 0, needed = 0, set;
    unsigned long rPTime = 0, cPTime = 0, rUpdate = 0, cUpdate = 0, waitTime = CORE_FRQ/5;
    
    unsigned int length = 1;
    unsigned char reading[length];
    SPIFLASH_Init();
    
    //read angle from SPI memory
    SPIFLASH_Read(SPIFLASH_PROG_ADDR, reading, length);
    angle = (int)reading[1]-45;
    
    //limit angle to -45 and 45 boundries
    angle = (angle < -45) ? -45 : angle;
    angle = (angle > 45) ? 45 : angle;
    
    servo_init(PB_FRQ, 2, angle);
    
    shell_init(PB_FRQ);
    shell_sendMsg("");
    
    char received[RX_BUF_SIZE];
    char command[RX_BUF_SIZE];
    
    srf_init(PB_FRQ);
    srf_registerDistanceCallback(srf_Callback);
    _CP0_SET_COUNT(0);
    macro_enable_interrupts();
    
    //execution
    while(1){
        if(stringDone == 1){
            memset(command,0,sizeof(command));
            memset(received,0,sizeof(received));
            shell_receiveMsg(received);
            strncat(command, received, 8);
            if(strcmp(command, "setangle") == 0){
                set = atoi(strstr(received, " "));
                if(set < -45 || set > 45){
                    shell_sendMsg("\r\nContent Error, angle is not within range of -45 to 45");
                    
                }else{
                    angle = set;
                    sendAngle(angle);
                }
            }else if(strcmp(command, "getangle") == 0){
                sendAngle(angle);
            }else if(strcmp(command, "getdistance")){
                sendDistance();
            }else{
                shell_sendMsg("\r\nSyntax Error");
            }
        }
        int r = debounce(BTNR, &rPTime, &rState, &rPState);
        int c = debounce(BTNC, &cPTime, &cState, &cPState);
        
        if(angle != pAngle){
            pAngle = angle;
            needed = 1;
            servo_setpos(2,angle);
        }
        
        if(rState == 0 && cState == 0 && needed == 1){
            needed = 0;
                //erase data in SPIFLASH
                SPIFLASH_Erase4k(SPIFLASH_PROG_ADDR);
                //write angle to SPIFLASH
                reading[1] = angle+45;               
                SPIFLASH_ProgramPage(SPIFLASH_PROG_ADDR, reading, length);
            }
        
        //if button R is pressed turn the servo right
        if(rState == 1 && cState == 0){
            if((_CP0_GET_COUNT()-rUpdate >= waitTime) && (angle < 45)){    
                rUpdate = _CP0_GET_COUNT();
                angle++;
            }
        }
        
        //if button C is pressed turn the servo left
        if(rState == 0 && cState == 1){
            if((_CP0_GET_COUNT()-cUpdate >= waitTime) && (angle > -45)){
                cUpdate = _CP0_GET_COUNT();
                angle--;
            }
        }
    }
}

void sendAngle(int angle){
    char send[RX_BUF_SIZE];
    char angleStr[RX_BUF_SIZE];
    sprintf(angleStr, "%i", angle);
    strncat(send, "\n\rServo is set to ", 20);
    strncat(send, angleStr, 10);
    strncat(send, " Degrees", 20);
    shell_sendMsg(send);
}

void sendDistance(){
    char send[RX_BUF_SIZE];
    char distanceStr[RX_BUF_SIZE];
    sprintf(distanceStr, "%i", distance);
    strncat(send, "\n\rMeasured distance is ", 20);
    strncat(send, distanceStr, 10);
    strncat(send, " cm", 20);
    shell_sendMsg(send);
}

void srf_Callback(unsigned short int dist){
    distance = dist;
}

void GPIO_init(){
    //BTNR
    TRISBbits.TRISB8 = 1;
    ANSELBbits.ANSB8 = 0;
    //BTNC
    TRISFbits.TRISF0 = 1;
}