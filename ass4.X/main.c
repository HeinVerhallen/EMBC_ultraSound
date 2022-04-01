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
#include "Debounce.h"
#include "srf.h"

int distance = 0;

void main(){
    //initialization varibles
    int angle = 0, previousAngle = 0, currentStateBtnR = 0, previousStateBtnR = 0, currentStateBtnC = 0, previousStateBtnC = 0, memoryUpdateNeeded = 0, set;
    unsigned long lastUpdateTimeBtnR = 0, lastUpdateTimeBtnC = 0, angleUpdate = 0;
    unsigned char reading[length];
    
    //initialize all components
    SPIFLASH_Init();
    servo_init(PB_FRQ, 2, angle);
    shell_init(PB_FRQ);
    GPIO_init();
    srf_init(PB_FRQ);
    
    //read angle from SPI memory
    SPIFLASH_Read(SPIFLASH_PROG_ADDR, reading, length);
    angle = (int)reading[1]-flashAngleOffset;
    
    //send opening line to uart
    shell_sendMsg("");
    
    srf_registerDistanceCallback(srf_Callback);
    
    _CP0_SET_COUNT(0);
    macro_enable_interrupts();
    
    //execution
    while(1){
        //limit angle to minAngle and maxAngle boundries
        angle = (angle < minAngle) ? minAngle : angle;
        angle = (angle > maxAngle) ? maxAngle : angle;
        
        updateUart(stringDone, &angle);

        debounce(BTNR, &lastUpdateTimeBtnR, &currentStateBtnR, &previousStateBtnR);
        debounce(BTNC, &lastUpdateTimeBtnC, &currentStateBtnC, &previousStateBtnC);
        
        if(angle != previousAngle){
            previousAngle = angle;
            memoryUpdateNeeded = 1;
            servo_setpos(2,angle);
        }

        if(currentStateBtnR && currentStateBtnC){
        } 
        else if((currentStateBtnR || currentStateBtnC) && _CP0_GET_COUNT()-angleUpdate >= waitTime){
            angle += 1 * currentStateBtnR + -1 *currentStateBtnC;
            angleUpdate = _CP0_GET_COUNT();
        }
        else if(memoryUpdateNeeded){
            memoryUpdateNeeded = 0;
            updateFlashMemory(reading, angle);
        }
    }
}

void sendMessage(int angle){
    char send[RX_BUF_SIZE];
    char angleStr[RX_BUF_SIZE];
    sprintf(angleStr, "%i", angle);
    strncat(send, "\n\rServo is set to ", 20);
    strncat(send, angleStr, 10);
    strncat(send, " Degrees", 10);
    shell_sendMsg(send);
}

void sendDistance(int distance){
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
    sendDistance(distance);
}

void GPIO_init(){
    //BTNR
    TRISBbits.TRISB8 = 1;
    ANSELBbits.ANSB8 = 0;
    //BTNC
    TRISFbits.TRISF0 = 1;
}

void updateUart(int stringDone, int* angle){
    //if string is not done exit function
    if(stringDone != 1) {
        return;
    }
    
    //setup command and received variables
    static char command[RX_BUF_SIZE], received[RX_BUF_SIZE];
    memset(command,0,sizeof(command));
    memset(received,0,sizeof(received));
    
    shell_receiveMsg(received);
    strncat(command, received, 8);

    if(strcmp(command, "setangle") == 0){
        int set = atoi(strstr(received, " "));
        if(set < minAngle || set > maxAngle){
            shell_sendMsg("\r\nContent Error, angle is not within range of -45 to 45");
            return;
        }
        *angle = set;
        sendMessage(*angle);
        return;
    }
    
    if(strcmp(command, "getangle") == 0){
        sendMessage(*angle);
        return;
    }
    
    memset(command,0,sizeof(command));
    strncat(command, received, 11);
    if(strcmp(command, "getdistance") == 0){
        srf_range();
        return;
    }
    shell_sendMsg("\r\nSyntax Error");
    return;
}

void updateFlashMemory(char reading[length], int angle){
    //Write the updated angle the the SPIFLASH memory
    SPIFLASH_Erase4k(SPIFLASH_PROG_ADDR);
    reading[1] = angle+flashAngleOffset;               
    SPIFLASH_ProgramPage(SPIFLASH_PROG_ADDR, reading, length);
}