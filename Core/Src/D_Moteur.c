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
void F_PWM_SetCmdMotorDroit(int speed){
	int dir=0;
	// Bornage de la donnée d'entrée
	if(speed<-100) speed= -100;
	if(speed> 100) speed=  100;

	// Commande du pont en H
	if(speed < 0){
		dir = -1;
	}else{
		dir = 1;
	}
	F_GPIO_SetMotorDroitDir(dir);
	F_PWM_SetDCMotorDroit(speed*dir);
}
/**
 *	@param[in] new_pwm_value new Duty cycle value in percentage %
 */
void F_PWM_SetCmdMotorGauche(int speed){
	int dir=0;
	// Bornage de la donnée d'entrée
	if(speed<-100) speed= -100;
	if(speed> 100) speed=  100;

	// Commande du pont en H
	if(speed < 0){
		(dir = -1);
	}else{
		(dir = 1);
	}
	F_GPIO_SetMotorGaucheDir(dir);
	F_PWM_SetDCMotorGauche(speed*dir);
}
/**
 *	@param[in] new_pwm_value new Duty cycle value in percentage %
 *
 *	Validé sur NUCLEO F429 ZI le 18-09-2018
 */
void F_PWM_SetDCMotorDroit(int dc){
	F_TIM4_CH3_SetDC(dc);
}
/**
 *	@param[in] new_pwm_value new Duty cycle value in percentage %
 *
 *	Validé sur NUCLEO F429 ZI le 18-09-2018
 */
void F_PWM_SetDCMotorGauche(int dc){
	F_TIM4_CH4_SetDC(dc);
}
