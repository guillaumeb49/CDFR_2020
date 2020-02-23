/*
 * F_Motion.c
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */


#include "F_Motion.h"

#include "usart.h"
#include "gpio.h"
#include "cmsis_os.h"
#include "D_Moteur.h"
#include "F_Odometry.h"

/*
 * F_Motion_MotorSpeedRegulator - Manual command
 */
void F_Motion_PolarSpeedRegulator(int fwrdSpeed_mmPerSec,int rotSpeed_mmPerSec){
	float l_fwrdSpeed;		// Linear speed from sensors
	float l_rotSpeed;		// Rotation speed from sensors
	float l_fwrdSpeedError;	// Linear speed computed error
	float l_rotSpeedError;	// Rotation speed computed error

	int l_cmd_left;		// Left  motor command value
	int l_cmd_right;	// Right motor command value

	// Données d'entée de régulateur
	F_Odometry_getPolarspeed(&l_fwrdSpeed,&l_rotSpeed);

	// Calcul des erreurs
	l_fwrdSpeedError  = fwrdSpeed_mmPerSec  - l_fwrdSpeed;
	l_rotSpeedError = rotSpeed_mmPerSec - l_rotSpeed;

	// Calcul de la commande
	l_cmd_left  = (l_fwrdSpeedError * KP_FWD_SPEED) - (l_rotSpeedError * KP_ROT_SPEED);
	l_cmd_right = (l_fwrdSpeedError * KP_FWD_SPEED) + (l_rotSpeedError * KP_ROT_SPEED);

	// Application de la commande
	D_Motor_SetCmdMotorGauche(l_cmd_left);
	D_Motor_SetCmdMotorDroit(l_cmd_right);
}
/*
 * F_Motion_ManualPilot - Manual command
 */
void F_Motion_ManualPilot(char a_consigne){
	int speed = 20;
	switch(a_consigne){
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
	case 'o'://F_Motion_PolarSpeedRegulator(0,0);
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

    // 2. Get tick count
    prevTick = osKernelSysTick();

    while(1){
        // 2. Wait until period elapse
    	osDelay(REG_TECH);
    	F_GPIO_SetLed2(TRUE);	// Flag On

        // 3. Get tick and compute period
        tick = osKernelSysTick();
        period = tick - prevTick;

        //4. Compute location
        F_Motion_ManualPilot(F_UART_GetReceivedChar());

        // 5. Set previous tick
        prevTick = tick;
        F_GPIO_SetLed2(FALSE);	// Flag Off
    }
}
