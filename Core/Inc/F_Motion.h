/*
 * F_Motion.h
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */

#ifndef INC_F_MOTION_H_
#define INC_F_MOTION_H_

#define REG_TECH 50
#define KP_FWD_SPEED 0.1
#define KP_ROT_SPEED 1

void F_Motion_PolarSpeedRegulator(int fwrdSpeed_mmPerSec,int rotSpeed_mmPerSec);
void F_Motion_ManualPilot(char);
void F_Motion_RegTask_Handler(void const * argument);

#endif /* INC_F_MOTION_H_ */
