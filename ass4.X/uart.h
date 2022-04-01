#ifndef _UART_H
#define _UART_H

/**
 * Platform: Basys MX3
 * Authors: Robert Konarskis, Michal Mikolajczyk, Digilent
 * 
 * This library provides functions for handling UART comminucation over the USB port.
 */

/**
 * External functions
 */

/**
 * Callback called when data is received on UART.
 */
typedef void (*UartCallback)(unsigned char *rx);
void registerUartCallback(UartCallback ptr_uartCallback);

/**
 * Initialize UART.
 * 
 * @param baud - integer
 *        UART baudrate.
 */
void UART_Init(unsigned int baud);
/**
 * Initialize UART for polling mode of operation.
 * 
 * @param baud - integer
 *        UART baudrate.
 */
void UART_InitPoll(unsigned int baud);
/**
 * Send a single char over UART.
 * 
 * @param ch - char
 *        Char to send over UART.
 */
void UART_PutChar(char ch);

/**
 * Clear UART input buffer
 */
void UART_ClearBuffer();

/**
 * Send a string of data over UART.
 * 
 * @param szData - char[]
 *        An array of characters representing the string.
 */
void UART_PutString(char szData[]);

/**
 * Stop UART communication.
 */
void UART_CloseUART();

/**
 * Internal functions
 */
#define	cchRxMax	0xFF	// maximum number of characters a CR+LF terminated string



void UART_ConfigurePins();
void UART_ConfigureUart(unsigned int baud);
void UART_ConfigureUartRXInt(unsigned int baud);

unsigned char UART_GetCharPoll();
unsigned char UART_AvaliableRx();
unsigned char UART_GetString( char* pchBuff, int cchBuff );
unsigned char UART_GetStringPoll(unsigned char *pText);

#endif /* _UART_H */