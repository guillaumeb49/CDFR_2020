/*
 * D_Moteur.h
 *
 *  Created on: 6 févr. 2020
 *      Author: mario
 */

#ifndef INC_D_MOTEUR_H_
#define INC_D_MOTEUR_H_

#include "main.h"

void Init_Motors(void);
void F_PWM_SetCmdMotorDroit(int speed);
void F_PWM_SetCmdMotorGauche(int speed);
void F_PWM_SetDCMotorDroit(int dc);
void F_PWM_SetDCMotorGauche(int dc);

#endif /* INC_D_MOTEUR_H_ */
