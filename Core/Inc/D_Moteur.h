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
void D_Motor_SetCmdMotorDroit(int relativeDC);
void D_Motor_SetCmdMotorGauche(int relativeDC);

#endif /* INC_D_MOTEUR_H_ */
