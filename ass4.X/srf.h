#ifndef _SRF_H
#define _SRF_H

/**
 * Platform: Basys MX3
 * Authors: Robert Konarskis, Michal Mikolajczyk
 * 
 * This library provides basic functionality for controlling the SRF-08 ultrasonic sensor.
 * 
 * Note: this library uses Timer 3.
 */

/**
 * External functions
 */

/**
 * Callback called when distance is acquired from SRF-08 module.
 */
typedef void (*SrfDistanceCallback)(unsigned char distance);
void srf_registerDistanceCallback(SrfDistanceCallback ptr_srfDistanceCallback);

/**
 * Initialize SRF-08 module.
 * 
 * @param freq - integer
 *        Peripheral bus clock frequency.    
 */
void srf_init(int freq);

/**
 * Send command to SRF-08 to start ranging.
 */
void srf_range();

/**
 * Internal functions
 */
unsigned char srf_startRanging();
unsigned char srf_getDistance(unsigned short int *distance);

#endif /* _SRF_H */