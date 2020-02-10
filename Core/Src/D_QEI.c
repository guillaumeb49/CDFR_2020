/*
 * D_QEI.c
 *
 *  Created on: Feb 5, 2020
 *      Author: mario
 */


#include "D_QEI.h"
#include "tim.h"


int g_oldLeftCount;
int g_oldRightCount;

int g_leftCount;
int g_rightCount;

int g_leftSpeed;
int g_rightSpeed;

float g_fwdSpeed;
float g_rotSpeed;

////State vector
//Localisation g_estimate;
//
///*
// * Init_QEI Initialize timers 2 and timer 3 as encoder mode
// * Init_QEI Initialize data to zero
// */
void Init_QEI(void){
	HAL_TIM_Encoder_Init(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Init(&htim2, TIM_CHANNEL_2);
	HAL_TIM_Encoder_Init(&htim3, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Init(&htim3, TIM_CHANNEL_2);

	F_QEI_Reset();
}
///*
// * F_QEI_Reset Localization data are reseted to zero;
// */
void F_QEI_Reset(void){
	__disable_irq();

	g_oldLeftCount	= 0;
	g_oldRightCount	= 0;
	g_leftSpeed		= 0;
	g_rightSpeed	= 0;
//
//	g_estimate.x = RBT_OF7_X;
//	g_estimate.y = RBT_OF7_Y;
//	g_estimate.teta = 0;

	TIM2->CNT=0;
	TIM3->CNT=0;

	F_QEI_Read();
	__enable_irq();
}
///*
// * F_QEI_Read is called every Timer 9 interrupt to update encoder's data
// */
void F_QEI_Read (void){

	g_oldLeftCount  = g_leftCount ;
	g_oldRightCount = g_rightCount;

	g_rightCount = TIM2->CNT ;
	g_leftCount  = TIM3->CNT ;

	g_leftSpeed  = g_leftCount  - g_oldLeftCount ;
	g_rightSpeed = g_rightCount - g_oldRightCount;

	// Si on passe par zero en phase descendante
	if(g_leftSpeed  > 4096) g_leftSpeed = -(g_oldLeftCount  + (65535-g_leftCount));
	if(g_rightSpeed > 4096) g_rightSpeed= -(g_oldRightCount + (65535-g_rightCount));
	// Si on passe par zero en phase montante
	if(g_leftSpeed  < -4096) g_leftSpeed = (g_leftCount + (65535  - g_oldLeftCount));
	if(g_rightSpeed < -4096) g_rightSpeed= (g_rightCount + (65535 - g_oldRightCount));

	g_fwdSpeed = (float)( g_leftSpeed + g_rightSpeed ) / 2;
	g_rotSpeed =  g_rightSpeed - g_leftSpeed;

	//F_QEI_XYTetaProcessing();
}
///*
// * F_QEI_XYTetaProcessing - Update localization state vector
// */
//void F_QEI_XYTetaProcessing(void){
//	g_estimate.x +=  TICKTOMM * g_fwdSpeed  * cosf(g_estimate.teta);
//	g_estimate.y +=  TICKTOMM * g_fwdSpeed  * sinf(g_estimate.teta);
//	g_estimate.teta +=  tanf( TICKTOMM * g_rotSpeed  / ENTRAXE ) ;
//
//	F_Math_ModuloPi(&g_estimate.teta);
//}
///*
// * F_QEI_getspeed - Return speed of each motor
// */
//void F_QEI_getspeed(float * left, float * right){
//	*left  = (float)g_leftSpeed  * TICKTOMM * FREQ_ECH; // In mm per s
//	*right = (float)g_rightSpeed * TICKTOMM * FREQ_ECH;
//}
///*
// * F_QEI_getEstimatedState - Update structure state as estimated state
// */
//void F_QEI_getEstimatedState(Localisation * state){
//	state->x=g_estimate.x;
//	state->y=g_estimate.y;
//	state->teta=g_estimate.teta;
//}
///*
// * F_QEI_printEstimatedState - Print estimated position
// */
//void F_QEI_printEstimatedState(void){
//	printf("X: %d Y: %d Teta: %d \n\r",(int)g_estimate.x,(int)g_estimate.y,(int)g_estimate.teta);
//}
///*
// * F_QEI_printEstimatedSpeed - Print estimated speed in Qt form
// */
//void F_QEI_printEstimatedSpeed(void){
//	float l_tmp_Rs,l_tmp_Ls;
//	F_QEI_getspeed(&l_tmp_Ls,&l_tmp_Rs);
//	printf("L: %d R: %d \n\r",(int)l_tmp_Ls,(int)l_tmp_Rs);
//}
///*
// * F_QEI_getCounters - Print tick count for each wheel
// */
//void F_QEI_printCounters(void){
//	printf("RegLeft: %d RegRight: %d \n\r",(int)g_leftCount,(int)g_rightCount);
//}
///*
// * F_QEI_getCounters - Print tick count for each wheel
// */
//void F_QEI_printCountersLCD(void){
//	char line1[16];
//	char line2[16];
//	sprintf(line1,"RegLeft : %5d",(int)g_leftCount);
//	sprintf(line2,"RegRight: %5d",(int)g_rightCount);
//	//F_LCD_clear();
//	F_LCD_setCursor(0,0);
//	F_LCD_printstr(line1);
//	F_LCD_setCursor(0,1);
//	F_LCD_printstr(line2);
//}
///*
// * F_QEI_setLocalisation - Set position and angle
// */
//void F_QEI_setLocalisation( Localisation Loc){
//	__disable_irq();
//
//	g_estimate.x = Loc.x ;
//	g_estimate.y = Loc.y ;
//	g_estimate.teta = Loc.teta ;
//
//	__enable_irq();
//}
///*
// * F_QEI_setLocalisation - Set position and angle
// */
//void F_QEI_setPosition( int x_mm, int y_mm, int teta_deg){
//	__disable_irq();
//
//	g_estimate.x = x_mm ;
//	g_estimate.y = y_mm ;
//
//	if(teta_deg < 360){
//		g_estimate.teta = F_Math_toRad( (float) teta_deg);
//	}else{
//		g_estimate.teta = 0;
//	}
//
//	__enable_irq();
//}
