/*
 * D_Moteur.c
 *
 *  Created on: 6 févr. 2020
 *      Author: mario
 */


#include "D_Moteur.h"

#include "tim.h"
#include "gpio.h"

void Init_Motors(void){
	F_GPIO_SetEnableMotors(1);

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
}
/**
 * @brief Initialize PWMs connected to Motor G
 *
 * Cela permet de s'interfacer avec les moteurs via une commande +100/-100
 * Pour faire l'automatique c'est obligatoire
 *
 */
void D_Motor_SetCmdMotorDroit(int relativeDC){
	int dir=0;
	// Bornage de la donnée d'entrée
	if(relativeDC<-100) relativeDC= -100;
	if(relativeDC> 100) relativeDC=  100;

	// Commande du pont en H
	if(relativeDC < 0){
		dir = -1;
	}else{
		dir = 1;
	}
	F_GPIO_SetMotorDroitDir(dir);
	D_TIM4_CH4_SetDC(relativeDC*dir);
}
/**
 *	@param[in] new_pwm_value new Duty cycle value in percentage %
 */
void D_Motor_SetCmdMotorGauche(int relativeDC){
	int dir=0;
	// Bornage de la donnée d'entrée
	if(relativeDC<-100) relativeDC= -100;
	if(relativeDC> 100) relativeDC=  100;

	// Commande du pont en H
	if(relativeDC < 0){
		(dir = -1);
	}else{
		(dir = 1);
	}
	F_GPIO_SetMotorGaucheDir(dir);
	D_TIM4_CH3_SetDC(relativeDC*dir);
}
