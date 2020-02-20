/*
 * F_Motion.h
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */

#ifndef INC_F_MOTION_H_
#define INC_F_MOTION_H_

#define REG_TECH 50
#define KP_SPEED 1
void F_Motion_MotorsSpeedRegulator(int a_leftSpeed_mmPerSec, int a_rightSpeed_mmPerSec);
void F_Motion_ManualPilot(char);
void F_Motion_RegTask_Handler(void const * argument);

#endif /* INC_F_MOTION_H_ */
