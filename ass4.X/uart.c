#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "uart.h"

// global variables, to communicate between interrupt handler and other
volatile unsigned char fRxDone;
volatile unsigned char ichRx;
char rgchRx[cchRxMax];

#define RX_BUF_SIZE 64
char rx[RX_BUF_SIZE];

UartCallback uartCallback;
unsigned char uartCallbackFlag = 0;

void registerUartCallback(UartCallback ptr_uartCallback) {
    uartCallback = ptr_uartCallback;
    uartCallbackFlag = 1;
}

void UART_Init(unsigned int baud) {
    UART_ConfigurePins();
    UART_ConfigureUartRXInt(baud);
}

void __ISR(_UART_4_VECTOR, ipl6auto) Uart4Handler (void) {
	unsigned char bVal;
	
	while(U4STAbits.URXDA) {
		bVal = (unsigned char) U4RXREG;
        
        if (bVal != '\b') {
            int i;
            for (i = RX_BUF_SIZE - 1; i > 0; i--) {
                rx[i] = rx[i - 1];
            }
        
            rx[0] = bVal;
        } else {
            int i;
            for (i = 0; i < RX_BUF_SIZE - 1; i++) {
                rx[i] = rx[i + 1];
            }
        }
	}
    
    if (uartCallbackFlag) {
        uartCallback(rx);
    }
    
	IFS2bits.U4RXIF = 0;
}

void UART_ClearBuffer() {
    int i = 0;
    while(rx[i] != '\0') {
        rx[i] = '\0';
        i++;
    }
}

void UART_ConfigureUart(unsigned int baud) {
    U4MODEbits.ON     = 0;
    U4MODEbits.SIDL   = 0;
    U4MODEbits.IREN   = 0; 
    U4MODEbits.RTSMD  = 0;
    U4MODEbits.UEN0   = 0; 
    U4MODEbits.UEN1   = 0;
    U4MODEbits.WAKE   = 0;
    U4MODEbits.LPBACK = 0; 
    U4MODEbits.ABAUD  = 0;
    U4MODEbits.RXINV  = 0; 
    U4MODEbits.PDSEL1 = 0; 
    U4MODEbits.PDSEL0 = 0; 
    U4MODEbits.STSEL  = 0;  

    
    U4MODEbits.BRGH   = 0; 

    U4BRG = (int)(((float)PB_FRQ/(16*baud) - 1) + 0.5); // add 0.5 just in order to implement the round using the floor approach

    U4STAbits.UTXEN    = 1;
    U4STAbits.URXEN    = 1;
    U4MODEbits.ON      = 1; 
    
}

void UART_ConfigureUartRXInt(unsigned int baud) {

    UART_ConfigureUart(baud);

    IPC9bits.U4IP = 6;
    IPC9bits.U4IS = 3;

	IFS2bits.U4RXIF = 0;    //Clear the Uart4 interrupt flag.
    IEC2bits.U4RXIE = 1;    // enable RX interrupt
    

    macro_enable_interrupts();  // enable interrupts 
}

void UART_ConfigurePins() {
    tris_UART_TX = 0;   //TX digital output
    rp_UART_TX = 2;     // 0010 U4TX
    
    tris_UART_RX = 1;   //RX digital input
    rp_UART_RX = 9;     // 1001 RF13
}

void UART_PutChar(char ch) {
    while(U4STAbits.UTXBF == 1);
    U4TXREG = ch;
}

void UART_PutString(char szData[]) {
    char *pData = szData;
    while(*pData) {
        UART_PutChar((*(pData++)));
    }
}

unsigned char UART_AvaliableRx() {
    return U4STAbits.URXDA;
}

unsigned char UART_GetCharPoll() {
    while (!UART_AvaliableRx());
    return U4RXREG;
}

unsigned char UART_GetStringPoll(unsigned char *pText) {
    int idx = 0;
    while(UART_AvaliableRx()) {
        pText[idx++] = UART_GetCharPoll();
    }
    if(idx != 0) {
        pText[idx] = 0; // terminator
    }
    return idx != 0;
}

unsigned char UART_GetString( char* pchBuff, int cchBuff ) {
	unsigned char ich;
	
	// Have we finished receiving a CR+LF terminated string via UART4?
	if(!fRxDone)
	{
		return 0;
	}
	// Does the user buffer have enough space to store the CR+LF terminated string?
	if(cchBuff < ichRx - 1)
	{
		// A buffer underrun occured.
		macro_disable_interrupts;
		fRxDone = 0;
		ichRx = 0;
		macro_enable_interrupts();
		
		return -2;
	}
    // Was a 0 character CR+LF terminated string received?
	if(2 == ichRx )
	{
		// A zero character length CR+LF terminated string was received.
		macro_disable_interrupts;
		fRxDone = 0;
		ichRx = 0;
		macro_enable_interrupts();
		return -3;
	}
	
    // copy the received chars to the destination location.
	for(ich = 0; ich < ichRx - 2; ich++)
	{
		*pchBuff = rgchRx[ich]; 
		pchBuff++;
	}
	*pchBuff = '\0';
	
	macro_disable_interrupts;
	fRxDone = 0;
	ichRx = 0;
	macro_enable_interrupts();
	return ich;
}

void UART_Close() {
      U4MODEbits.ON = 0;   
}
/***	UART_InitPoll
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud			
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the UART module, in 
**      the UART receive without interrupts (polling method).
**      The UART_TX digital pin is configured as digital output.
**      The UART_RX digital pin is configured as digital input.
**      The UART_TX and UART_RX are mapped over the UART4 interface.
**      The UART4 module of PIC32 is configured to work at the specified baud, no parity and 1 stop bit.
**      
**          
*/
void UART_InitPoll(unsigned int baud)
{
    UART_ConfigurePins();
    UART_ConfigureUart(baud);
}
