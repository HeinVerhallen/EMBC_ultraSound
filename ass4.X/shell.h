/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef shell_H    /* Guard against multiple inclusion */
#define shell_H

int stringDone;

void shell_UARTcallback(unsigned char *rx);

/* ========================================================================== */
/* function: shell_init( fpb ); */
/* */
/* description: initialises the shell module. */
/* */
/* pre: fpb - peripheral bus clock frequency in Hz */
/* */
/* post: return value - 0 if initialisation failed */
/* 1 if initialisation succeeded */
/* ========================================================================== */
unsigned char shell_init
(
unsigned int fpb
);
/* ========================================================================== */
/* function: shell_sendMsg( message ); */
/* */
/* description: transmits a message, stored as a string in the message */
/* variable, to the terminal. The message will be terminated */
/* with a newline: "\n\r". */
/* */
/* pre: message - pointer to the first element of the string to be */
/* transmitted to the terminal */
/* */
/* post: return value - 0 if transmission failed */
/* 1 if transmission was successful */
/* ========================================================================== */
unsigned char shell_sendMsg
(
unsigned char *message
);
/* ========================================================================== */
/* function: shell_ReceiveMsge( message ); */
/* */
/* description: receives a message, stored as a string in the message */
/* variable, send to the shell. The message will be terminated */
/* with a newline: "\n\r". */
/* */
/* pre: message - pointer to the first element of the string to be */
/* received from the shell */
/* */
/* post: return value - 0 if reception of msg not complete */
/* 1 if reception was of msg is completed */
/* ========================================================================== */
unsigned char shell_receiveMsg
(
unsigned char *message
);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
