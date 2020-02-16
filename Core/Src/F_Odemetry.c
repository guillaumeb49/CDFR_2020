/*
 * F_Odemetry.c
 *
 *  Created on: Feb 1, 2020
 *      Author: mario
 */
#include "F_Odometry.h"

#include "D_QEI.h"
#include "cmsis_os.h"

void F_Odometry_Compute(uint32_t period_inTick){
	F_QEI_Read();
	//	g_oldLeftCount  = g_leftCount ;
	//	g_oldRightCount = g_rightCount;
	//
	//	g_rightCount = TIM2->CNT ;
	//	g_leftCount  = TIM3->CNT ;
	//
	//	g_leftSpeed  = g_leftCount  - g_oldLeftCount ;
	//	g_rightSpeed = g_rightCount - g_oldRightCount;
	//
	//	// Si on passe par zero en phase descendante
	//	if(g_leftSpeed  > 4096) g_leftSpeed = -(g_oldLeftCount  + (65535-g_leftCount));
	//	if(g_rightSpeed > 4096) g_rightSpeed= -(g_oldRightCount + (65535-g_rightCount));
	//	// Si on passe par zero en phase montante
	//	if(g_leftSpeed  < -4096) g_leftSpeed = (g_leftCount + (65535  - g_oldLeftCount));
	//	if(g_rightSpeed < -4096) g_rightSpeed= (g_rightCount + (65535 - g_oldRightCount));
	//
	//	g_fwdSpeed = (float)( g_leftSpeed + g_rightSpeed ) / 2;
	//	g_rotSpeed =  g_rightSpeed - g_leftSpeed;
	//
	//	g_estimate.x +=  TICKTOMM * g_fwdSpeed  * cosf(g_estimate.teta);
	//	g_estimate.y +=  TICKTOMM * g_fwdSpeed  * sinf(g_estimate.teta);
	//	g_estimate.teta +=  tanf( TICKTOMM * g_rotSpeed  / ENTRAXE ) ;
	//
	//	F_Math_ModuloPi(&g_estimate.teta);
}

void F_Odometry_RegTask_Handler(void const * argument){
    TickType_t prevTick = 0u;	// Save previous tick
    TickType_t tick = 0u;		// Current tick

    uint32_t period;

    // 1. Initialize QEI
    F_QEI_Reset();

    // 2. Get tick count
    prevTick = osKernelSysTick();

    while(1){
        // 2. Wait until period elapse
    	osDelay(ODO_TECH);
    	F_GPIO_SetLed1(TRUE);	// Flag On

        // 3. Get tick and compute period
        tick = osKernelSysTick();
        period = tick - prevTick;

        //4. Compute location
        F_Odometry_Compute(period);

        // 5. Set previous tick
        prevTick = tick;
        F_GPIO_SetLed1(FALSE);	// Flag Off
    }
}
