/*
 * F_Odemetry.c
 *
 *  Created on: Feb 1, 2020
 *      Author: mario
 */
#include "F_Odometry.h"
#include "usart.h"
#include "F_LCD.h"
#include "F_Maths.h"
#include "tim.h"
#include <stdio.h>

#include "cmsis_os.h"

int g_oldLeftCount;
int g_oldRightCount;

//State vector
Localisation g_estimate;
DynamicState g_speedVector;
/*
 * F_Odometry_Init Initialize timers 2 and timer 3 as encoder mode
 * F_Odometry_Init Initialize data to zero
 */
void F_Odometry_Init(void){
	F_Odometry_Reset();
}
/*
 * F_Odometry_Reset Localization data are reseted to zero;
 */
void F_Odometry_Reset(void){
	HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);

	g_oldLeftCount	= 0;
	g_oldRightCount	= 0;

	g_estimate.x_tick = 0;
	g_estimate.y_tick = 0;
	g_estimate.teta = 0;

	TIM2->CNT=0;
	TIM3->CNT=0;

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}
/*
 * F_Odometry_Read get the timers counters and compute forward and rotation speed
 */
void F_Odometry_Read(float *leftDelta, float * rightDelta){
	int l_leftDelta, l_rightDelta;
	int l_leftCount  = TIM2->CNT ;
	int l_rightCount = TIM3->CNT ;

	l_leftDelta  = l_leftCount  - g_oldLeftCount ;
	l_rightDelta = l_rightCount - g_oldRightCount;

	// Si on passe par zero en phase descendante
	if(l_leftDelta  > 4096) l_leftDelta = -(g_oldLeftCount  + (65535-l_leftCount));
	if(l_rightDelta > 4096) l_rightDelta= -(g_oldRightCount + (65535-l_rightCount));
	// Si on passe par zero en phase montante
	if(l_leftDelta  < -4096) l_leftDelta = (l_leftCount + (65535  - g_oldLeftCount));
	if(l_rightDelta < -4096) l_rightDelta= (l_rightCount + (65535 - g_oldRightCount));

	*leftDelta  = l_leftDelta*WHEEL_C;
	*rightDelta = l_rightDelta;

	g_oldLeftCount  = l_leftCount ;
	g_oldRightCount = l_rightCount;
}
/*
 * F_Odometry_getLocalisation - Return position
 */
void F_Odometry_getLocalisation(int * x_mm, int * y_mm, int *teta_deg){
	*x_mm=g_estimate.x_tick*TICKTOMM;
	*y_mm=g_estimate.y_tick*TICKTOMM;
	*teta_deg=F_Math_toDeg(g_estimate.teta);
}
/*
 * F_Odometry_getPolarspeed - Return speed of each motor
 */
void F_Odometry_getPolarspeed(float * fwrdSpeed_mmPerSec, float * rotSpeed_degPerSec){
	*fwrdSpeed_mmPerSec = g_speedVector.fwrdSpeed_mmPerSec;
	*rotSpeed_degPerSec = g_speedVector.rotSpeed_degPerSec;
}
/*
 * F_Odometry_getWheelSpeed - Return speed of each motor
 */
void F_Odometry_getWheelSpeed(float * leftSpeed_mmPerSec, float * rightSpeed_mmPerSec){
	*leftSpeed_mmPerSec = g_speedVector.leftSpeed_mmPerSec;
	*rightSpeed_mmPerSec= g_speedVector.rightSpeed_mmPerSec;
}
/*
 * F_Odometry_Compute - Compute the estimate position of the robot from motion capture.
 */
void F_Odometry_Compute(uint32_t period_ms){
	float l_fwrdDeltick;	// Forward speed in tick per sample period
	float l_rotDeltick;		// Angle speed in tick per sample period
	float l_rotDelrad;		// Angle speed in rad per sample period
	float l_rightDeltick;	// Right speed in tick per sample period
	float l_leftDeltick;	// Left speed in tick per sample period

	F_Odometry_Read(&l_leftDeltick,&l_rightDeltick);

	l_fwrdDeltick = (l_leftDeltick + l_rightDeltick) / 2.0;
	l_rotDeltick =  l_rightDeltick - l_leftDeltick;
	l_rotDelrad = tanf(l_rotDeltick/ENTR_TICK);

	g_estimate.x_tick +=  l_fwrdDeltick * cosf(g_estimate.teta);
	g_estimate.y_tick +=  l_fwrdDeltick  * sinf(g_estimate.teta);
	g_estimate.teta +=  l_rotDelrad;

	F_Math_ModuloPi(&g_estimate.teta);

	g_speedVector.fwrdSpeed_mmPerSec = (l_fwrdDeltick  *TICKTOMM) * (1000.0 / period_ms);
	g_speedVector.rotSpeed_degPerSec = F_Math_toDeg(l_rotDelrad) * (1000.0 / period_ms);
//	g_speedVector.rightSpeed_mmPerSec= 1000 * (l_rightDeltick *TICKTOMM) / period_ms;
//	g_speedVector.leftSpeed_mmPerSec = 1000 * (l_leftDeltick  *TICKTOMM) / period_ms;
}
/*
 * F_Odometry_printEstimatedSpeed - Print estimated speed
 */
void F_Odometry_printEstimatedSpeed(void){
	float l_fwds,l_rots;
	char line1[60];

	F_Odometry_getPolarspeed(&l_fwds, &l_rots);
	sprintf(line1,"FwdS: %d RotS: %d C:%c\n\r",(int)l_fwds,(int)l_rots,F_UART_GetReceivedChar());
	F_UART_Send(line1);
}
/*
 * F_Odometry_printCounters - Print tick count for each wheel
 */
void F_Odometry_printCountersUART(void){
	char line1[60];
	sprintf(line1,"RegLeft: %d RegRight: %d C: %c\n\r",g_oldLeftCount,g_oldRightCount,F_UART_GetReceivedChar());
	F_UART_Send(line1);
}
/*
 * F_Odometry_printCounters - Print tick count for each wheel
 */
void F_Odometry_printPositionUART(void){
	char line1[60];
	int l_x,l_y,l_teta;
	F_Odometry_getLocalisation(&l_x, &l_y, &l_teta);
	sprintf(line1,"X:%5d Y:%5d Teta:%4d C:%c \n\r",l_x,l_y,l_teta,F_UART_GetReceivedChar());
	F_UART_Send(line1);
}
/*
 * F_Odometry_printCountersLCD - Print tick count for each wheel
 */
void F_Odometry_printCountersLCD(void){
	char line1[16];
	char line2[16];
	sprintf(line1,"RegLeft : %5d",(int)g_oldLeftCount);
	sprintf(line2,"RegRight: %5d",(int)g_oldRightCount);
	//F_LCD_clear();
	F_LCD_setCursor(0,0);
	F_LCD_printstr(line1);
	F_LCD_setCursor(0,1);
	F_LCD_printstr(line2);
}
/*
 * F_Odometry_printCountersLCD - Print tick count for each wheel
 */
void F_Odometry_printPositionLCD(void){
//	char line1[16];
//	char line2[16];
//	sprintf(line1,"X:%5d Y:%5d",(int)g_estimate.x_tick,(int)g_estimate.y_mm);
//	sprintf(line2,"Teta: %5d",(int)F_Math_toDeg(g_estimate.teta));
//
//	//F_LCD_clear();
//	F_LCD_setCursor(0,0);
//	F_LCD_printstr(line1);
//	F_LCD_setCursor(0,1);
//	F_LCD_printstr(line2);
}
/*
 * F_Odometry_RegTask_Handler - task in charge of localization based on QEI.
 */
void F_Odometry_RegTask_Handler(void const * argument){
    TickType_t prevTick = 0u;	// Save previous tick
    TickType_t tick = 0u;		// Current tick

    uint32_t period;

    // 1. Initialize QEI
    F_Odometry_Init();

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
