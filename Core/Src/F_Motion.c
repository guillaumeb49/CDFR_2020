/*
 * F_Motion.c
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */


#include "F_Motion.h"

#include "usart.h"
#include "gpio.h"
#include "D_Moteur.h"
#include "F_Odometry.h"

/*
 * F_Motion_PolarRegulator - Manual command
 */
void F_Motion_PolarRegulator(int16_t angle_deg,PID *PID_Fwrd, PID*PID_Rot){
	int16_t l_teta;		// Angle
	int16_t l_tetaError;// Angle error
	int16_t l_buff;		// buffer

	int16_t l_rotSpeed;	// Rotation speed set point

	// Données d'entée de régulateur
	F_Odometry_getLocalisation(&l_buff, &l_buff, &l_teta);

	// Calcul des erreurs
	l_tetaError = angle_deg - l_teta;

	// Calcul de la commande
	l_rotSpeed  = l_tetaError * 0.5;

	// Application de la commande
	F_Motion_SpeedRegulator(0,l_rotSpeed,PID_Fwrd,PID_Rot);
}
/*
 * F_Motion_MotorSpeedRegulator
 */
void F_Motion_SpeedRegulator(int16_t fwrdSpeed_mmPerSec,int16_t rotSpeed_degPerSec,PID *PID_Fwrd, PID*PID_Rot){
	float l_fwrdSpeed;		// Linear speed from sensors
	float l_rotSpeed;		// Rotation speed from sensors
	float l_fwrdSpeedError;	// Linear speed computed error
	float l_rotSpeedError;	// Rotation speed computed error

	float l_fwrdCorrection, l_rotCorrection;

	int8_t l_cmd_left=0;		// Left  motor command value
	int8_t l_cmd_right=0;		// Right motor command value

	// Données d'entrée de régulateur
	F_Odometry_getPolarspeed(&l_fwrdSpeed,&l_rotSpeed);

	// Calcul des erreurs
	l_fwrdSpeedError = fwrdSpeed_mmPerSec  - l_fwrdSpeed;
	l_rotSpeedError  = rotSpeed_degPerSec - l_rotSpeed;

	// Calcul de l'intégral
	PID_Fwrd->ErSum += l_fwrdSpeedError;
	PID_Rot->ErSum  += l_rotSpeedError;
	// Calcul de la variation de l'erreur
	PID_Fwrd->ErVar = l_fwrdSpeedError - PID_Fwrd->OldEr;
	PID_Rot->ErVar  = l_rotSpeedError  - PID_Rot->OldEr;
	PID_Fwrd->OldEr = l_fwrdSpeedError;
	PID_Rot->OldEr = l_rotSpeedError;

	// Calcul de la commande
	l_fwrdCorrection = (l_fwrdSpeedError * PID_Fwrd->KP) + (PID_Fwrd->ErSum * PID_Fwrd->KI) + (PID_Fwrd->ErVar * PID_Fwrd->KD);
	l_rotCorrection  = (l_rotSpeedError  * PID_Rot->KP ) + (PID_Rot->ErSum  * PID_Rot->KI)  + (PID_Rot->ErVar  * PID_Rot->KD);

	l_cmd_left  = (l_fwrdCorrection - l_rotCorrection) ;
	l_cmd_right = (l_fwrdCorrection + l_rotCorrection) ;

//	// Application de la commande
	D_Motor_SetCmdMotorGauche(l_cmd_left);
	D_Motor_SetCmdMotorDroit(l_cmd_right);
}
/*
 * F_Motion_ManualPilot - Manual command
 */
void F_Motion_ManualPilot(char state, PID *PID_Fwrd, PID *PID_Rot){
	int speed = 10;

	switch(state){
	case 'z':D_Motor_SetCmdMotorDroit(speed);
			 D_Motor_SetCmdMotorGauche(speed);
		break;
	case 's':D_Motor_SetCmdMotorDroit(-speed);
			 D_Motor_SetCmdMotorGauche(-speed);
		break;
	case 'q':D_Motor_SetCmdMotorDroit(speed);
			 D_Motor_SetCmdMotorGauche(-speed);
		break;
	case 'd':D_Motor_SetCmdMotorDroit(-speed);
			 D_Motor_SetCmdMotorGauche(speed);
		break;
	case 'o':F_Motion_SpeedRegulator(100,0,PID_Fwrd,PID_Rot);
		break;
	case 'l':F_Motion_SpeedRegulator(-100,0,PID_Fwrd,PID_Rot);
		break;
	case 'k':F_Motion_SpeedRegulator(0,60,PID_Fwrd,PID_Rot);
		break;
	case 'm':F_Motion_SpeedRegulator(0,-60,PID_Fwrd,PID_Rot);
		break;
	case 'i':F_Motion_SpeedRegulator(0,20,PID_Fwrd,PID_Rot);
		break;
	case 'p':F_Motion_SpeedRegulator(0,-20,PID_Fwrd,PID_Rot);
		break;
	case 'f':F_Motion_PolarRegulator(-90, PID_Fwrd, PID_Rot);
		break;
	case 'g':F_Motion_PolarRegulator(0, PID_Fwrd, PID_Rot);
		break;
	case 'h':F_Motion_PolarRegulator(90, PID_Fwrd, PID_Rot);
		break;
	case 'r':F_Odometry_Reset();
		break;
	default :
			D_Motor_SetCmdMotorDroit(0);
			D_Motor_SetCmdMotorGauche(0);
	}
}
/*
 * F_Motion_RegTask_Handler - task in charge of motion gesture
 */
void F_Motion_RegTask_Handler(void const * argument){
    TickType_t prevTick = 0u;	// Save previous tick
    TickType_t tick = 0u;		// Current tick

    uint32_t period;

    // 1. Initialize QEI
	Init_Motors();

	PID l_fwrSpeedPID;
	PID l_rotSpeedPID;

//	l_fwrSpeedPID.KP 	= 0.15;
//	l_fwrSpeedPID.KI 	= 0.01;
//	l_fwrSpeedPID.KD 	= 0.005;
	l_fwrSpeedPID.KP 	= 0;
	l_fwrSpeedPID.KI 	= 0;
	l_fwrSpeedPID.KD 	= 0;
	l_fwrSpeedPID.ErSum = 0.0;
	l_fwrSpeedPID.ErVar = 0.0;
	l_fwrSpeedPID.OldEr = 0.0;

	l_rotSpeedPID.KP 	= 0.5;
	l_rotSpeedPID.KI 	= 0.05;
	l_rotSpeedPID.KD 	= 0.0;
	l_rotSpeedPID.ErSum = 0.0;
	l_rotSpeedPID.ErVar = 0.0;
	l_rotSpeedPID.OldEr = 0.0;

    // 2. Get tick count
    prevTick = osKernelSysTick();


    while(1){
        // 2. Wait until period elapse
    	//osDelay(REG_TECH);
    	osDelayUntil(&prevTick,REG_TECH);
    	F_GPIO_SetLed2(TRUE);	// Flag On

        // 3. Get tick and compute period
        tick = osKernelSysTick();
        period = tick - prevTick;

        //4. Compute
        F_Motion_ManualPilot(F_UART_GetReceivedChar(),&l_fwrSpeedPID,&l_rotSpeedPID);

        // 5. Set previous tick
        prevTick = tick;
        F_GPIO_SetLed2(FALSE);	// Flag Off
    }
}
