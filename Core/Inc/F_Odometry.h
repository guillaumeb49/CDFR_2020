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

// Electronic parameters
#define TICKTOMM 	PI*WHEEL_D/QEI_RES	// Move from tick to millimeter
#define ODO_TECH 	5			 		// Update period in millisecond

void F_Odometry_Compute(uint32_t period_inTick);
void F_Odometry_RegTask_Handler(void const * argument);


#endif /* SRC_F_ODOMETRY_H_ */
