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
#define ENTR	 	265					// Interval between wheels in millimeter
#define ENTR_TICK 	2757.1530527763994	// QEI_RES*265/(WHEEL_D*PI) ENTR*MMTOTICK
#define WHEEL_D		73.42540792540792	// Wheel diameter
#define WHEEL_R		WHEEL_D/2			// Wheel half diameter
#define WHEEL_C		1.0			// Wheel size difference correction
#define QEI_RES		2400		// 600*4 = Resolution par tour x nb de front
// Electronic parameters
#define TICKTOMM 	PI*WHEEL_D/QEI_RES	// Move from tick to millimeter
#define MMTOTICK 	QEI_RES/PI*WHEEL_D	// Move from tick to millimeter
#define ODO_TECH 	5			 		// Update period in millisecond

struct localisation {
	float x_tick;
	float y_tick;
	float x_mm;
	float y_mm;
	float teta;
};
typedef struct localisation Localisation;

void F_Odometry_Compute(uint32_t period_inTick);
void F_Odometry_RegTask_Handler(void const * argument);

void F_Odometry_Init(void);
void F_Odometry_Reset(void);
void F_Odometry_Read(float *fwrdProgress_tick, float *rotProgress_tick);
void F_Odometry_getspeed(float * leftSpeed, float * rightSpeed);
void F_Odometry_getLocalisation(int * x_mm, int * y_mm, int *teta_deg);
void F_Odometry_printEstimatedSpeed(void);
void F_Odometry_printCountersUART(void);
void F_Odometry_printPositionUART(void);
void F_Odometry_printCountersLCD(void);
void F_Odometry_printPositionLCD(void);

#endif /* SRC_F_ODOMETRY_H_ */
