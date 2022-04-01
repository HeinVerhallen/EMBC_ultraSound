
#ifndef DEBOUNCE_H    /* Guard against multiple inclusion */
#define DEBOUNCE_H

#include "stdint.h"

// Function for debouncing a button input
// Parameters:
//		buttonPin	the pin to which the button is connected
// Returns HIGH once if the button is pressed, LOW otherwise
int debounce(uint8_t button, unsigned long *lastDebounceTime, int *buttonState, int *lastButtonState);

#endif /* DEBOUNCE_H */

/* *****************************************************************************
 End of File
 */