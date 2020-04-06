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

RobotState g_RobotState ;

//State vector
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

	g_RobotState.pos_tick.x = 0;
	g_RobotState.pos_tick.y = 0;
	g_RobotState.pos_tick.teta = 0;
	g_RobotState.fwrdSpeed_mmPerSec= 0;
	g_RobotState.rotSpeed_degPerSec= 0;
	g_RobotState.leftSpeed_mmPerSec= 0;
	g_RobotState.rightSpeed_mmPerSec=0;
	g_RobotState.rotSpeed_lowPassCoef=0.2;

	TIM2->CNT=0;
	TIM3->CNT=0;

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}
/*
 * F_Odometry_Read get the timers counters and compute forward and rotation speed
 */
void F_Odometry_Read(float *leftDelta, float * rightDelta){
	int32_t l_leftDelta, l_rightDelta;
	uint32_t l_leftCount  = TIM2->CNT ;
	uint32_t l_rightCount = TIM3->CNT ;

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
void F_Odometry_getLocalisation(int16_t * x_mm, int16_t * y_mm, int16_t *teta_deg){
	*x_mm=(int16_t) g_RobotState.pos_tick.x*TICKTOMM;
	*y_mm=(int16_t) g_RobotState.pos_tick.y*TICKTOMM;
	*teta_deg=(int16_t) F_Math_toDeg(g_RobotState.pos_tick.teta);
}
/*
 * F_Odometry_getPolarspeed - Return speed of each motor
 */
void F_Odometry_getPolarspeed(float * fwrdSpeed_mmPerSec, float * rotSpeed_degPerSec){
	*fwrdSpeed_mmPerSec = g_RobotState.fwrdSpeed_mmPerSec;
	*rotSpeed_degPerSec = g_RobotState.rotSpeed_degPerSec;
}
/*
 * F_Odometry_getWheelSpeed - Return speed of each motor
 */
void F_Odometry_getWheelSpeed(float * leftSpeed_mmPerSec, float * rightSpeed_mmPerSec){
	*leftSpeed_mmPerSec = g_RobotState.leftSpeed_mmPerSec;
	*rightSpeed_mmPerSec= g_RobotState.rightSpeed_mmPerSec;
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
	float l_fwrdSpeedRaw ;
	float l_rotSpeedRaw ;
	float l_fwrdSpeedFiltered ;
	float l_rotSpeedFiltered ;
	float l_lowPassCoef = g_RobotState.fwrdSpeed_lowPassCoef;

	F_Odometry_Read(&l_leftDeltick,&l_rightDeltick);

	l_fwrdDeltick = (l_leftDeltick + l_rightDeltick) / 2.0;
	l_rotDeltick =  l_rightDeltick - l_leftDeltick;
	l_rotDelrad = tanf(l_rotDeltick/ENTR_TICK);

	g_RobotState.pos_tick.x +=  l_fwrdDeltick * cosf(g_RobotState.pos_tick.teta);
	g_RobotState.pos_tick.y +=  l_fwrdDeltick * sinf(g_RobotState.pos_tick.teta);
	g_RobotState.pos_tick.teta +=  l_rotDelrad;

	F_Math_ModuloPi(&g_RobotState.pos_tick.teta);

	if(period_ms !=0){
		l_fwrdSpeedRaw = (l_fwrdDeltick*TICKTOMM)  * (1000.0 / period_ms);
		l_rotSpeedRaw  = F_Math_toDeg(l_rotDelrad) * (1000.0 / period_ms);
	}

	l_fwrdSpeedFiltered = (l_fwrdSpeedRaw*(1-l_lowPassCoef)) + (g_RobotState.fwrdSpeed_mmPerSec * l_lowPassCoef);
	l_rotSpeedFiltered  = (l_rotSpeedRaw *(1-l_lowPassCoef)) + (g_RobotState.rotSpeed_degPerSec * l_lowPassCoef);

	g_RobotState.fwrdSpeed_mmPerSec = l_fwrdSpeedFiltered;
	g_RobotState.rotSpeed_degPerSec = l_rotSpeedFiltered ;
	g_RobotState.rightSpeed_mmPerSec= (l_rightDeltick *TICKTOMM) * (1000.0 / period_ms);
	g_RobotState.leftSpeed_mmPerSec = (l_leftDeltick  *TICKTOMM) * (1000.0 / period_ms);
}
/*
 * F_Odometry_printEstimatedSpeed - Print estimated speed
 */
void F_Odometry_printEstimatedSpeed(void){
	float l_fwds,l_rots;
	char line1[60];

	F_Odometry_getPolarspeed(&l_fwds, &l_rots);
	sprintf(line1,"FwdS: %d RotS: %d C:%c\n\r",(int)l_fwds,(int)l_rots,F_UART_GetReceivedChar());
	F_UART_Send(line1,60);
}
/*
 * F_Odometry_printCounters - Print tick count for each wheel
 */
void F_Odometry_printCountersUART(void){
	char line1[40];
	sprintf(line1,"RegL:%d RegR:%d C:%c\n\r",g_oldLeftCount,g_oldRightCount,F_UART_GetReceivedChar());
	F_UART_Send(line1,40);
}
/*
 * F_Odometry_printCounters - Print tick count for each wheel
 */
void F_Odometry_printPositionUART(void){
	char line1[60];
	int16_t l_x=0,l_y,l_teta;
	F_Odometry_getLocalisation(&l_x, &l_y, &l_teta);
//	sprintf(line1,"Bonjour\n\r");//l_x);
//	sprintf(line1,"X:%d\n\r",l_x);
//	sprintf(line1,"X:%5d Y:%5d\n\r",l_x,l_y);
	sprintf(line1,"X%5d Y%5d T:%4d C:%c\n\r",l_x,l_y,l_teta,F_UART_GetReceivedChar());
	F_UART_Send(line1,60);
}
/*
 * F_Odometry_printCountersLCD - Print tick count for each wheel
 */
void F_Odometry_printCountersLCD(void){
	char line1[16];
	char line2[16];
	sprintf(line1,"RegL:%5d",(int)g_oldLeftCount);
	sprintf(line2,"RegR:%5d",(int)g_oldRightCount);
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
osStatus g_test;
void F_Odometry_RegTask_Handler(void const * argument){
    uint32_t prevTick = 0u;	// Save previous tick
    uint32_t tick = 0u;		// Current tick

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
