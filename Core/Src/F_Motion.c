/*
 * F_Motion.c
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */


#include "F_Motion.h"

#include "usart.h"
#include "gpio.h"
#include "D_Moteur.h"
#include "F_Odometry.h"
#include "F_Maths.h"

///////////////////////////////////////////////////////////////
// Calcul du cap à suivre pour aller d'un point à l'autre
// @start - Point de départ
// @target  - Point d'arrivée
////////////////////////////////////////////////////////////////
static float F_Motion_capTo(Position start,Position target);
///////////////////////////////////////////////////////////////
// Prise en compte des limitations mécaniques du robot
// @a_fwrdSpeed - Pointeur sur la consigne de vitesse linéaire
// @a_rotSpeed  - Pointeur sur la consigne de vitesse angulaire
// @a_max_fwrdSpeed - Vitesse linéaire maximale
// @a_max_rotSpeed  - Vitesse angulaire maximale
////////////////////////////////////////////////////////////////
static int F_Motion_fitToRobot(	int16_t* a_fwrdSpeed, int16_t* a_rotSpeed,int16_t a_max_fwrdSpeed,int16_t a_max_rotSpeed);
///////////////////////////////////////////////////////////////
// Asservissement en cap et vitesse
// @fwrdSpeed_mmPerSec	: Consigne de vitesse linéique (en mm par seconde)
// @rotSpeed_degPerSec	: Consigne en vitesse angulaire (en degrés par seconde)
// @PID_Fwrd 	: Coeficients du PID pour la vitesse linéique
// @PID_Rot 	: Coeficients du PID pour la vitesse angulaire
////////////////////////////////////////////////////////////////
static int F_Motion_SpeedRegulator(int16_t fwrdSpeed_mmPerSec,int16_t rotSpeed_degPerSec, PID* PID_Fwrd, PID* PID_Rot);
///////////////////////////////////////////////////////////////
// Asservissement en position
// @a_MyPos		: Position actuelle
// @a_tgtPoint	: Position desirée
// @a_priority 	: Priorité en DELAY,ORIENTATION (Asservissement en cap dans le sens du signe) ou SPEEDSIGN (Marche Avant ou Marche Arrière)
//			DELAY ->		Déplacement le plus rapide
//			ORIENTATION -> 	Asservissement en cap dans le sens du signe de l'option puis déplacement
// 			SPEEDSIGN ->	Déplacement en marche avant ou marche arrière selon le signe de l'option
// @a_option 	: Paramètre de la priorité
// @PID_Fwrd	: Structure des coefs et mémoires du PID en distance
// @PID_Rot		: Structure des coefs et mémoires du PID angulaire
////////////////////////////////////////////////////////////////
static int F_Motion_TrackPoint(Position a_MyPos, Position a_tgtPoint,e_MOTION_PRIORITY a_priority, float a_option,PID *PID_Fwrd,PID*PID_Rot);


/*
 * F_Motion_ManualPilot - Manual command
 */
void F_Motion_ManualPilot(char state, PID *PID_Fwrd, PID *PID_Rot){
	int speed = 10;
	Position l_tgt;

	switch(state){
	case 'z':D_Motor_SetCmdMotorDroit(speed);
			 D_Motor_SetCmdMotorGauche(speed);
		break;
	case 's':D_Motor_SetCmdMotorDroit(-speed);
			 D_Motor_SetCmdMotorGauche(-speed);
		break;
	case 'q':D_Motor_SetCmdMotorDroit(speed);
			 D_Motor_SetCmdMotorGauche(-speed);
		break;
	case 'd':D_Motor_SetCmdMotorDroit(-speed);
			 D_Motor_SetCmdMotorGauche(speed);
		break;
	case 'o':F_Motion_SpeedRegulator(100,0,PID_Fwrd,PID_Rot);
		break;
	case 'l':F_Motion_SpeedRegulator(-100,0,PID_Fwrd,PID_Rot);
		break;
	case 'b':
			l_tgt.x = 100;
			l_tgt.y = 100;
			l_tgt.teta = 0;
			F_Motion_TrackPoint(F_Odometry_getPosition(), l_tgt, DELAY, 0,PID_Fwrd,PID_Rot);
			break;
	case 'r':F_Odometry_Reset();
		break;
	default :
			D_Motor_SetCmdMotorDroit(0);
			D_Motor_SetCmdMotorGauche(0);
	}
}
/*
 * F_Motion_RegTask_Handler - task in charge of motion gesture
 */
void F_Motion_RegTask_Handler(void const * argument){
    TickType_t prevTick = 0u;	// Save previous tick
    TickType_t tick = 0u;		// Current tick

    uint32_t period;

    // 1. Initialize QEI
	Init_Motors();

	PID l_fwrSpeedPID;
	PID l_rotSpeedPID;

	l_fwrSpeedPID.KP 	= 0.15;
	l_fwrSpeedPID.KI 	= 0.01;
	l_fwrSpeedPID.KD 	= 0.005;
//	l_fwrSpeedPID.KP 	= 0;
//	l_fwrSpeedPID.KI 	= 0;
//	l_fwrSpeedPID.KD 	= 0;
	l_fwrSpeedPID.ErSum = 0.0;
	l_fwrSpeedPID.ErVar = 0.0;
	l_fwrSpeedPID.OldEr = 0.0;

	l_rotSpeedPID.KP 	= 0.5;
	l_rotSpeedPID.KI 	= 0.05;
	l_rotSpeedPID.KD 	= 0.0;
	l_rotSpeedPID.ErSum = 0.0;
	l_rotSpeedPID.ErVar = 0.0;
	l_rotSpeedPID.OldEr = 0.0;

    // 2. Get tick count
    prevTick = osKernelSysTick();


    while(1){
        // 2. Wait until period elapse
    	osDelayUntil(&prevTick,REG_TECH);
    	F_GPIO_SetLed2(TRUE);	// Flag On

        // 3. Get tick and compute period
        tick = osKernelSysTick();
        period = tick - prevTick;

        //4. Compute
        F_Motion_ManualPilot(F_UART_GetReceivedChar(),&l_fwrSpeedPID,&l_rotSpeedPID);

        // 5. Set previous tick
        prevTick = tick;
        F_GPIO_SetLed2(FALSE);	// Flag Off
    }
}

static int F_Motion_TrackPoint(	Position a_MyPos, Position a_tgtPoint,
								e_MOTION_PRIORITY a_priority, float a_option,
								PID *PID_Fwrd,PID*PID_Rot){
	// Paramètres du régulateur
	float l_Kp_Dist = 1;
	float l_Kp_Ang	= 1;
	float l_precisionDistance  = 2 ;
	float l_precisionAngulaire = 0.95 ;
	// Sorties du régulateur
	int16_t l_fwrdSpeed_consigne ;
	int16_t l_rotSpeed_consigne ;
	// Variables temporaires du régulateur
	float l_errDistance;
	float l_errAngulaire;

	// Vérification des paramètres d'entrée
	if(a_option >  1) a_option =  1;
	if(a_option < -1) a_option = -1;
	if( !((a_priority=!DELAY) || (a_priority!=ORIENTATION) || (a_priority!=SPEEDSIGN))) a_priority = DELAY;

	// Calcul de l'erreur en distance
	l_errDistance = F_Math_Distance(a_MyPos, a_tgtPoint);
	// Calcul de l'erreur angulaire
	if(l_errDistance < l_precisionDistance){
		l_errDistance = 0;
		l_errAngulaire = F_Math_ModuloPi(a_tgtPoint.teta - a_MyPos.teta);
	}else{
		l_errAngulaire = F_Math_ModuloPi(F_Motion_capTo(a_MyPos, a_tgtPoint)- a_MyPos.teta);
	}
	// Calcul des consignes de vitesse linéaire et angulaire en fonction de la priorité

	switch(a_priority){
	case DELAY :
			if( (F_Math_Abs(l_errAngulaire) > (PI/2)) && (l_errDistance > l_precisionDistance)){
				// Marche Arrière
				l_fwrdSpeed_consigne = l_Kp_Dist * l_errDistance * (-1);
				l_rotSpeed_consigne  = l_Kp_Ang  * F_Math_ModuloPi(l_errAngulaire + PI);
			}else{
				// Marche Avant
				l_fwrdSpeed_consigne = l_Kp_Dist * l_errDistance;
				l_rotSpeed_consigne  = l_Kp_Ang * l_errAngulaire;
			}
		break;
	case ORIENTATION :
			if(cosf(l_errAngulaire) > l_precisionAngulaire){
				l_rotSpeed_consigne  = l_Kp_Ang  * l_errAngulaire;
				l_fwrdSpeed_consigne = l_Kp_Dist * l_errDistance;
			}else{
				l_fwrdSpeed_consigne = 0;
				if(F_Math_Sign(l_errAngulaire)==F_Math_Sign(a_option)){
					l_rotSpeed_consigne  = l_Kp_Ang  * l_errAngulaire;
				}else{
					l_rotSpeed_consigne  = l_Kp_Ang  * (F_Math_Abs(l_errAngulaire) + (a_option*PI_X2));
				}
			}
		break;
	case SPEEDSIGN :
			l_fwrdSpeed_consigne = l_Kp_Dist * l_errDistance * a_option;
			if(l_errDistance < l_precisionDistance){
				l_rotSpeed_consigne  = l_Kp_Ang * l_errAngulaire;
			}else{
				l_rotSpeed_consigne = l_Kp_Ang * F_Math_ModuloPi(l_errAngulaire + PI*((1-a_option)/2));
			}
		break;
	}

	F_Motion_fitToRobot(&l_fwrdSpeed_consigne,&l_rotSpeed_consigne, 1000, 360);
	// Application des consignes
	F_Motion_SpeedRegulator(l_fwrdSpeed_consigne, l_rotSpeed_consigne, PID_Fwrd, PID_Rot);
	return 0;
}

static int F_Motion_SpeedRegulator(	int16_t fwrdSpeed_mmPerSec,
									int16_t rotSpeed_degPerSec,
									PID *PID_Fwrd,
									PID*PID_Rot){

	// Variables temporaires du régulateur
	float l_fwrdSpeed;		// Linear speed from sensors
	float l_rotSpeed;		// Rotation speed from sensors
	float l_fwrdSpeedError;	// Linear speed computed error
	float l_rotSpeedError;	// Rotation speed computed error
	float l_fwrdCorrection, l_rotCorrection;

	int8_t l_cmd_left=0;		// Left  motor command value
	int8_t l_cmd_right=0;		// Right motor command value

	// Données d'entrée de régulateur
	F_Odometry_getPolarspeed(&l_fwrdSpeed,&l_rotSpeed);

	// Calcul des erreurs
	l_fwrdSpeedError = fwrdSpeed_mmPerSec  - l_fwrdSpeed;
	l_rotSpeedError  = rotSpeed_degPerSec - l_rotSpeed;

	// Calcul de l'intégral
	PID_Fwrd->ErSum += l_fwrdSpeedError;
	PID_Rot->ErSum  += l_rotSpeedError;
	// Calcul de la variation de l'erreur
	PID_Fwrd->ErVar = l_fwrdSpeedError - PID_Fwrd->OldEr;
	PID_Rot->ErVar  = l_rotSpeedError  - PID_Rot->OldEr;
	PID_Fwrd->OldEr = l_fwrdSpeedError;
	PID_Rot->OldEr = l_rotSpeedError;

	// Calcul de la commande
	l_fwrdCorrection = (l_fwrdSpeedError * PID_Fwrd->KP) + (PID_Fwrd->ErSum * PID_Fwrd->KI) + (PID_Fwrd->ErVar * PID_Fwrd->KD);
	l_rotCorrection  = (l_rotSpeedError  * PID_Rot->KP ) + (PID_Rot->ErSum  * PID_Rot->KI)  + (PID_Rot->ErVar  * PID_Rot->KD);

	l_cmd_left  = (l_fwrdCorrection - l_rotCorrection) ;
	l_cmd_right = (l_fwrdCorrection + l_rotCorrection) ;

	// Application de la commande
	D_Motor_SetCmdMotorGauche(l_cmd_left);
	D_Motor_SetCmdMotorDroit(l_cmd_right);

	return 0;
}

static int F_Motion_fitToRobot(	int16_t* a_fwrdSpeed,		// Pointeur sur la consigne de vitesse linéaire
						int16_t* a_rotSpeed,		// Pointeur sur la consigne de vitesse angulaire
						int16_t a_max_fwrdSpeed,	// Vitesse linéaire maximale
						int16_t a_max_rotSpeed){	// Vitesse angulaire maximale

	float l_rotSpeed_ratio ;	// Ratio consigne/vitesse max (suppression de l'unité)
	float l_fwrdSpeed_ratio ;	// Ratio consigne/vitesse max (suppression de l'unité)
	float l_vectPolaire_Norme ;	// Norme du vecteur vitesse
	float l_vectPolaire_Arg	  ;	// Argument du vecteur vitesse

	if( (a_max_fwrdSpeed==0) || (a_max_rotSpeed==0) ){	// Protection contre la division par zero
		return -1;
	}

	l_rotSpeed_ratio  	= (*a_rotSpeed  ) / a_max_rotSpeed;
	l_fwrdSpeed_ratio 	= (*a_fwrdSpeed ) / a_max_fwrdSpeed;
	l_vectPolaire_Norme = sqrtf( (l_fwrdSpeed_ratio*l_fwrdSpeed_ratio) + (l_rotSpeed_ratio*l_rotSpeed_ratio) );
	l_vectPolaire_Arg	= atan2f(l_fwrdSpeed_ratio,l_rotSpeed_ratio);

	if(l_vectPolaire_Norme > sqrtf(0.5)){
		l_vectPolaire_Norme = sqrtf(0.5);
	}

	*a_fwrdSpeed = l_vectPolaire_Norme * cosf(l_vectPolaire_Arg) * a_max_fwrdSpeed ;
	*a_rotSpeed  = l_vectPolaire_Norme * sinf(l_vectPolaire_Arg) * a_max_rotSpeed ;

	return 0;
}

static float F_Motion_capTo(Position start,Position target){
	float l_cap = atan2f(target.y - start.y, target.x - start.x);
	return l_cap;
}

