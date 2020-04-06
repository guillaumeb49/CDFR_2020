/*
 * F_Motion.h
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */

#ifndef INC_F_MOTION_H_
#define INC_F_MOTION_H_

#define REG_TECH 	25

#include "cmsis_os.h"

struct pid {
	float KP;
	float KI;
	float KD;
	float ErSum;
	float ErVar;
	float OldEr;
};
typedef struct pid PID;

void F_Motion_PolarRegulator(int16_t angle_deg,PID* PID_Fwrd, PID* PID_Rot);
void F_Motion_SpeedRegulator(int16_t fwrdSpeed_mmPerSec,int16_t rotSpeed_degPerSec, PID* PID_Fwrd, PID* PID_Rot);
void F_Motion_ManualPilot(char, PID* PID_Fwrd, PID* PID_Rot);
void F_Motion_RegTask_Handler(void const * argument);

#endif /* INC_F_MOTION_H_ */
