/*
 * D_Moteur.h
 *
 *  Created on: 6 févr. 2020
 *      Author: mario
 */

#ifndef INC_D_MOTEUR_H_
#define INC_D_MOTEUR_H_

#define CMD_MOT_OFF7		12
#define CMD_MOT_OFF7_100	(1-(CMD_MOT_OFF7/100))
#include "cmsis_os.h"

void Init_Motors(void);
void D_Motor_SetCmdMotorDroit(int8_t relativeDC);
void D_Motor_SetCmdMotorGauche(int8_t relativeDC);

#endif /* INC_D_MOTEUR_H_ */
