/*
 * F_Odemetry.c
 *
 *  Created on: Feb 1, 2020
 *      Author: mario
 */
#include "F_Odometry.h"
#include "F_LCD.h"
#include "tim.h"
#include <stdio.h>

#include "cmsis_os.h"

int g_oldLeftCount;
int g_oldRightCount;

/*
 * F_Odometry_Init Initialize timers 2 and timer 3 as encoder mode
 * F_Odometry_Init Initialize data to zero
 */
void F_Odometry_Init(void){

//	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
//	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

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
//
//	g_estimate.x = RBT_OF7_X;
//	g_estimate.y = RBT_OF7_Y;
//	g_estimate.teta = 0;

	TIM2->CNT=0;
	TIM3->CNT=0;

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}
/*
 * F_Odometry_Read get the timers counters and compute forward and rotation speed
 */
void F_Odometry_Read(float *fwrdSpeed, float *rotSpeed){
	int l_leftSpeed;
	int l_rightSpeed;

	int l_leftCount  = TIM3->CNT ;
	int l_rightCount = TIM2->CNT ;

	l_leftSpeed  = l_leftCount  - g_oldLeftCount ;
	l_rightSpeed = l_rightCount - g_oldRightCount;

	// Si on passe par zero en phase descendante
	if(l_leftSpeed  > 4096) l_leftSpeed = -(g_oldLeftCount  + (65535-l_leftCount));
	if(l_rightSpeed > 4096) l_rightSpeed= -(g_oldRightCount + (65535-l_rightCount));
	// Si on passe par zero en phase montante
	if(l_leftSpeed  < -4096) l_leftSpeed = (l_leftCount + (65535  - g_oldLeftCount));
	if(l_rightSpeed < -4096) l_rightSpeed= (l_rightCount + (65535 - g_oldRightCount));

	*fwrdSpeed = (float)( l_leftSpeed + l_rightSpeed ) / 2;
	*rotSpeed =  l_rightSpeed - l_leftSpeed;

	g_oldLeftCount  = l_leftCount ;
	g_oldRightCount = l_rightCount;
	//F_QEI_XYTetaProcessing();
}
/*
 * F_Odometry_getspeed - Return speed of each motor
 */
void F_Odometry_getspeed(float * leftSpeed, float * rightSpeed, int period_ms){
	if(period_ms!=0){
//		*leftSpeed  = (float)leftSpeed  * TICKTOMM * (1/period_ms); // In mm per s
//		*rightSpeed = (float)g_rightSpeed * TICKTOMM * (1/period_ms);
	}
}
/*
 * F_Odometry_Compute - Compute the estimate position of the robot from motion capture.
 */
void F_Odometry_Compute(uint32_t period_inTick){
	float l_fwrdSpeed;	// Forward speed
	float l_rotSpeed;	// Angle speed

	F_Odometry_Read(&l_fwrdSpeed,&l_rotSpeed);
}
///*
// * F_Odometry_printEstimatedSpeed - Print estimated speed in Qt form
// */
void F_Odometry_printEstimatedSpeed(void){
	//float l_tmp_Rs,l_tmp_Ls;
	//F_Odometry_getspeed(&l_tmp_Ls,&l_tmp_Rs);
	//printf("L: %d R: %d \n\r",(int)l_tmp_Ls,(int)l_tmp_Rs);
}
/*
 * F_Odometry_printCounters - Print tick count for each wheel
 */
void F_Odometry_printCounters(void){
	//printf("RegLeft: %d RegRight: %d \n\r",(int)g_leftCount,(int)g_rightCount);
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
