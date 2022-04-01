#ifndef _SERVO_H
#define _SERVO_H

/**
 * Platform: Basys MX3
 * Authors: Hein Verhallen, Youri Tils
 * 
 * Description
 */

/* ========================================================================== */ 
/* function: servo_init( fpb, channel, angle );                               */ 
/*                                                                            */ 
/* description: initialises a servo channel.                                  */ 
/*                                                                            */ 
/* pre:  fpb           - peripheral bus clock frequency in Hz                 */ 
/*       channel       - number of the servo channel to initialise            */ 
/*                       (range 1...5)                                        */ 
/*       angle         - servo opening angle in degrees (range -45...45)      */ 
/*                                                                            */ 
/* post: return value  - 0 if initialisation failed, 1 if succeeded           */ 
/* ========================================================================== */ 
unsigned char servo_init 
( 
    int fpb, 
    int channel, 
    int angle 
); 
 
/* ========================================================================== */ 
/* function: servo_setpos( fpb, channel, angle );                             */ 
/*                                                                            */ 
/* description: changes the opening angle for a servo channel.                */ 
/*                                                                            */ 
/* pre:  channel       - number of the servo channel to set the position for  */ 
/*                       (range 1...5)                                        */ 
/*       angle         - servo opening angle in degrees (range -45...45)      */ 
/*                                                                            */ 
/* post: none                                                                 */ 
/* ========================================================================== */ 
void servo_setpos 
( 
    int channel, 
    int angle 
);

/* ========================================================================== */ 
/* function: servo_getWidth( angle );                                         */ 
/*                                                                            */ 
/* description: calculate the neccesairy pulse width of the PWM signal        */ 
/*                                                                            */ 
/* pre:  angle       - angle at which the servo motor should be set to        */ 
/*                       (range -45...45)                                     */ 
/*                                                                            */ 
/* post: width      - the width of the PWM signal is us                       */ 
/*                       (range 900...1500)                                   */ 
/* ========================================================================== */ 
int servo_getWidth
(
    int angle
);

#endif