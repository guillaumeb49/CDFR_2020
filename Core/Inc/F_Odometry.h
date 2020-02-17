/*
 * F_Odometry.h
 *
 *  Created on: Feb 1, 2020
 *      Author: mario
 */

#ifndef SRC_F_ODOMETRY_H_
#define SRC_F_ODOMETRY_H_

#include "main.h"

// Mechanical Parameters
#define ENTRAXE 	240	// Interval between wheels in millimeter
#define WHEEL_D		72
#define WHEEL_R		WHEEL_D/2
#define QEI_RES		2400	// 600*4 = Resolution par tour x nb de front
#define PI			3.14
// Electronic parameters
#define TICKTOMM 	PI*WHEEL_D/QEI_RES	// Move from tick to millimeter
#define ODO_TECH 	5			 		// Update period in millisecond

void F_Odometry_Compute(uint32_t period_inTick);
void F_Odometry_RegTask_Handler(void const * argument);

void F_Odometry_Init(void);
void F_Odometry_Reset(void);
void F_Odometry_Read(float *fwrdSpeed, float *rotSpeed);
void F_Odometry_getspeed(float * leftSpeed, float * rightSpeed, int period_ms);
void F_Odometry_printEstimatedSpeed(void);
void F_Odometry_printCounters(void);
void F_Odometry_printCountersLCD(void);

#endif /* SRC_F_ODOMETRY_H_ */
