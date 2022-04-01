/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include <string.h>
#include "shell.h"
#include "uart.h"


#define RX_BUF_SIZE 64
char buffer[RX_BUF_SIZE];
int stringDone = 0;
int ind = 0;


unsigned char shell_init(unsigned int fpb){
    unsigned int baud = 9600;
    UART_Init(baud);
    registerUartCallback(shell_UARTcallback);
    return 1;
}

unsigned char shell_sendMsg(unsigned char *message){
    UART_PutString(message);
    UART_PutString("\r\nEMBC>");
    return 1;
}

unsigned char shell_receiveMsg(unsigned char *message){
    if(stringDone == 1){
        strcpy(message, buffer);
        memset(buffer,0,sizeof(buffer)); // clearing buffer
        stringDone = 0;
    }
    return 1;
}

void shell_UARTcallback(unsigned char *rx)
{
    if(*rx == 0xd){
        stringDone = 1;
    }
    strncat(buffer, rx, 5);
    
    if(*rx == 0x7f){
        //delete previous character
        if(ind >= 1){
            buffer[ind--] = 0;
            buffer[ind--] = 0;
        }
    }
    ind++;
    UART_ClearBuffer();
    
    return;
}