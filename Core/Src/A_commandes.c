/*
 * A_commandes.c
 *
 *  Created on: 5 août 2018
 *      Author: guillaume
 */

#include "A_commandes.h"

//extern Localisation g_estimate;
extern int g_nbPoints;
extern int g_i_point;
extern int g_enableAUTO;

/**
 *
 */
void F_Process_Command(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	s_cmd_answer->id = s_cmd_received.id; 	// same ID as the received command
	s_cmd_answer->nb_octet = NB_OCTETS_CMD;
	s_cmd_answer->cmd = s_cmd_received.cmd;	// recall the command executed

	// Process the command depending on the command number
	switch(s_cmd_received.cmd)
	{
		// Get info
		case CMD_INFO:
			F_Cmd_Info(s_cmd_received, s_cmd_answer);
			break;

		// Set LED
		case CMD_SET_LED:
			status = F_Cmd_SetLED(s_cmd_received, s_cmd_answer);
			break;

		// Get LED
		case CMD_GET_LED:
			status = F_Cmd_GetLED(s_cmd_received, s_cmd_answer);
			break;
		// Get distances from sensors
		case CMD_GET_DISTANCES:
			F_Cmd_GetDistance(s_cmd_received, s_cmd_answer);
			break;

		// Set robot's position
		case CMD_SET_POSITION:
			F_Cmd_SetPosition(s_cmd_received, s_cmd_answer);
			break;

		// get robot's position
		case CMD_GET_POSITION:
			F_Cmd_GetPosition(s_cmd_received, s_cmd_answer);
			break;

		// Start navigation
		case CMD_GO:
			F_Cmd_StartStopRegulation(s_cmd_received, s_cmd_answer);
			break;

		// Add a new point to the list of points
		case CMD_ADD_POINT:
			F_Cmd_AddWayPoint(s_cmd_received, s_cmd_answer);
			break;

		// Remove one point from the list of points
		case CMD_RESET_LIST_WAYPOINTS:
			F_Cmd_ResetWayPointsList(s_cmd_received, s_cmd_answer);
			break;

		// Get the list of points
		case CMD_GET_LIST_POINTS:
			break;

		// Get the list of points
			case CMD_GET_TIRETTE:
				F_Cmd_GetTirette(s_cmd_received, s_cmd_answer);
			break;

		// Get the list of points
			case CMD_MOVE_SERVO:
				F_Cmd_MoveServo(s_cmd_received, s_cmd_answer);
			break;


		// Error, unknown command
		default:
			break;

	}


}



/**
 * Get the info and build the TCP packet accordingly
 * s_cmd_answer->reponse[0] = 0;	// current x
   s_cmd_answer->reponse[1] = 0;	// current y
   s_cmd_answer->reponse[2] = 0;	// current theta
   s_cmd_answer->reponse[3] = 0; --> 0 0 0 0 | 0 0 0 0

   reponse[3] :
       - bits [15 - 1] : current index #
       - bit 0 : Distance 1 & Distance 2 & Distance 3 ==> = 1 if distance < 30cm otherwise 0


 */
uint8_t F_Cmd_Info(Tcp_command s_cmd_received, Tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;
	uint8_t distance_warning = 0;
	if((distance_avant_1 < DISTANCE_WARNING) || (distance_avant_2 < DISTANCE_WARNING) || (distance_avant_3 < DISTANCE_WARNING))
	{
		distance_warning = 1;
	}


	s_cmd_answer->code_retour = status;
	/*s_cmd_answer->reponse[0] = (int16_t)(g_estimate.x);
	s_cmd_answer->reponse[1] = (int16_t)(g_estimate.y);
	s_cmd_answer->reponse[2] = (int16_t)(g_estimate.teta*1000);
	s_cmd_answer->reponse[3] = (uint16_t)((g_nbPoints << 1) | (0x01 & distance_warning));
*/
	return status;

}

/**
 * Set the on board LEDs
 */
uint8_t F_Cmd_SetLED(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;
	// All LEDs

	/*	F_GPIO_SetLedRed(0);
		F_GPIO_SetLedBlue(0);
		F_GPIO_SetLedGreen(0);

	// Red
	if(s_cmd_received.params[0] & 0x01)
	{
		F_GPIO_SetLedBlue(1);
	}
	// blue
	else if(s_cmd_received.params[0] == 2)
	{
		F_GPIO_SetLedGreen(1);
	}
	// Green
	else if(s_cmd_received.params[0] == 4)
	{
		F_GPIO_SetLedRed(1);
	}
	else
	{
		status = STATUS_ERROR_PARAM;
	}
*/
	s_cmd_answer->code_retour = status;
	s_cmd_answer->reponse[0] = 0;
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;

	return status;
}

/**
 * Set the on board LEDs
 */
uint8_t F_Cmd_GetLED(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;


	s_cmd_answer->code_retour = status;
	/*s_cmd_answer->reponse[0] = (((GPIOB->ODR & LED_Red) >> 14) << 2) + ((GPIOB->ODR & LED_Green) << 1) + (((GPIOB->ODR & LED_Blue) >> 7));
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;
*/
	return status;
}


/**
 * Set the on board LEDs
 */
uint8_t F_Cmd_GetDistance(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	s_cmd_answer->code_retour = status;
	s_cmd_answer->reponse[0] = distance_avant_1;
	s_cmd_answer->reponse[1] = distance_avant_2;
	s_cmd_answer->reponse[2] = distance_avant_3;
	s_cmd_answer->reponse[3] = distance_arriere_1;

	return status;
}

/**
 * Set the on board LEDs
 */
uint8_t F_Cmd_GetTirette(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	s_cmd_answer->code_retour = status;
	/*s_cmd_answer->reponse[0] = (uint16_t)(F_GPIO_GetTirette());
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;
*/
	return status;
}

/**
 * Set the on board LEDs
 */
uint8_t F_Cmd_MoveServo(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	if(s_cmd_received.params[0] == 0)
	{
	//	F_Servo_Fermer();
	}
	else
	{
	//	F_Servo_Ouvrir();
	}

	s_cmd_answer->code_retour = status;
	s_cmd_answer->reponse[0] = 0;
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;

	return status;
}



/**
 * Set the on board LEDs
 */
uint8_t F_Cmd_GetPosition(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	s_cmd_answer->code_retour = status;
/*	s_cmd_answer->reponse[0] = (int16_t)(g_estimate.x);
	s_cmd_answer->reponse[1] = (int16_t)(g_estimate.y);
	s_cmd_answer->reponse[2] = (int16_t)(g_estimate.teta*1000);
	s_cmd_answer->reponse[3] = 0;
*/
	return status;
}

/**
 * Set the position of the Robot
 */
uint8_t F_Cmd_SetPosition(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

//	F_QEI_setPosition((int16_t)s_cmd_received.params[0], (int16_t)s_cmd_received.params[1], (int16_t)s_cmd_received.params[2]);

	s_cmd_answer->code_retour = status;
	s_cmd_answer->reponse[0] = 0;
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;

	return status;
}


/**
 * Add a new WayPoint to the list of WayPoints
 */
uint8_t F_Cmd_AddWayPoint(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	//status = F_AUTO_AddTargetPoint((float)((int16_t)s_cmd_received.params[0]) , (float)((int16_t)s_cmd_received.params[1]), (float)((int16_t)s_cmd_received.params[2]));


	s_cmd_answer->code_retour = status;
	s_cmd_answer->reponse[0] = status;
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;

	return status;
}

/**
 * Start / Stop the regulation
 */
uint8_t F_Cmd_StartStopRegulation(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	// If param 1 = 1, then start regulation
	if(s_cmd_received.params[0] == 1)
	{
		//F_AUTO_Enable();
	}
	else
	{
		//F_AUTO_Disable();
	}

	s_cmd_answer->code_retour = status;
	//s_cmd_answer->reponse[0] = (uint16_t)g_enableAUTO;
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;

	return status;
}

/**
 * Reset the list of WayPoints
 */
uint8_t F_Cmd_ResetWayPointsList(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

//	F_AUTO_ResetTargetList();

	s_cmd_answer->code_retour = status;
	s_cmd_answer->reponse[0] = 0;
	s_cmd_answer->reponse[1] = 0;
	s_cmd_answer->reponse[2] = 0;
	s_cmd_answer->reponse[3] = 0;

	return status;
}

