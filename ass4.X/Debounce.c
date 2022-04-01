
#include "Debounce.h"

/* Generic C libraries */
#include <stdio.h>
#include <stdlib.h>

/* XC32 libraries */
#include <xc.h>
#include <sys/attribs.h>

#include "config.h"
#include "stdint.h"

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
const unsigned long debounceDelay = 50;		// the debounce time; increase if the output flickers

// Function for debouncing a button input
// Parameters:
//		buttonPin	the pin to which the button is connected
// Returns HIGH once if the button is pressed, LOW otherwise
int debounce(uint8_t button, unsigned long *lastDebounceTime, int *buttonState, int *lastButtonState)
{
	int result = 0;	// value to be returned after executing the function

    int reading = button;
	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH),  and you've waited
	// long enough since the last press to ignore any noise:

	// if the switch changed, due to noise or pressing:
	if (reading != *lastButtonState)
	{
		// reset the debouncing timer
		*lastDebounceTime = _CP0_GET_COUNT();
	}

	if (_CP0_GET_COUNT() > debounceDelay)
	{
		// whatever the reading is at, it's been there for longer than
		// the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != *buttonState)
		{
			*buttonState = reading;

			// only toggle the LED if the new button state is HIGH
			if (*buttonState == 1)
			{
				result = 1;
			}
		}
	}

	// save the reading.  Next time through the function,
	// it'll be the lastButtonState:
	*lastButtonState = reading;

	return result;
}



/* *****************************************************************************
 End of File
 */
