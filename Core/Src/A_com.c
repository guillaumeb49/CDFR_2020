/*
 * A_com.c
 *
 *  Created on: Feb 9, 2020
 *      Author: guillaume
 */


#include "A_com.h"





/*-----------------------------------------------------------------------------------*/
static void tcpecho_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  void *data;
  u16_t len;
  err_t recv_err;
  uint32_t array[NB_OCTETS_CMD_MAX] = {0};

  struct tcp_answer s_cmd_answer;
  struct tcp_command s_cmd_received;

  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);

  if (conn!=NULL)
  {
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, NULL, 7);

    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      netconn_listen(conn);

      while (1)
      {
        /* Grab new connection. */
         accept_err = netconn_accept(conn, &newconn);

        /* Process the new connection. */
        if (accept_err == ERR_OK)
        {
          recv_err = netconn_recv(newconn, &buf);
					while ( recv_err == ERR_OK)
          {
            do
            {
              netbuf_data(buf, &data, &len);
              F_TCP_paquetTocmd(data,len, &s_cmd_received);

              F_Process_Command(s_cmd_received,&s_cmd_answer);

        	  F_TCP_answerTotab(array, s_cmd_answer);

              netconn_write(newconn, array, sizeof(array), NETCONN_COPY);

            }
            while (netbuf_next(buf) >= 0);

            netbuf_delete(buf);
						recv_err = netconn_recv(newconn, &buf);
          }

          /* Close connection and discard connection identifier. */
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
    }
    else
    {
      netconn_delete(newconn);
      printf(" can not bind TCP netconn");
    }
  }
  else
  {
    printf("can not create TCP netconn");
  }
}
/*-----------------------------------------------------------------------------------*/

void tcpecho_init(void)
{
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/


/**
 *
 */
void F_Process_Command(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer)
{
	//uint8_t status = STATUS_OK;

	s_cmd_answer->id = s_cmd_received.id; 	// same ID as the received command
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
			F_Cmd_Set_LED(s_cmd_received, s_cmd_answer);
			break;

		// Get LED
		case CMD_GET_LED:

			break;
		// Get distances from sensors
		case CMD_GET_DISTANCES:

			break;

		// Set robot's position
		case CMD_SET_POSITION:

			break;

		// get robot's position
		case CMD_GET_POSITION:

			break;

		// Start navigation
		case CMD_GO:

			break;

		// Add a new point to the list of points
		case CMD_ADD_POINT:

			break;

		// Get the tirette status
		case CMD_GET_TIRETTE:

			break;

		// Get the list of points
		case CMD_MOVE_SERVO:

			break;

		// Get the list of points
		case CMD_MANUAL_CTRL:
			F_Cmd_Manual_Control(s_cmd_received, s_cmd_answer);
			break;

		// Get the UID of the STM32
		case CMD_GET_UID:
			F_Cmd_Get_UID(s_cmd_received, s_cmd_answer);
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

	int32_t x,y,theta;

	F_Odometry_getLocalisation(&x, &y, &theta);

	s_cmd_answer->code_retour = status;
	s_cmd_answer->nb_reponse = 9;
	s_cmd_answer->reponse[0] = (int32_t)x;	// Position X
	s_cmd_answer->reponse[1] = (int32_t)y;	// Position Y
	s_cmd_answer->reponse[2] = (int32_t)theta;  // Position Theta
	s_cmd_answer->reponse[3] = (int32_t)F_GetDistanceSensor(0);	// Capteur distance 1
	s_cmd_answer->reponse[4] = (int32_t)F_GetDistanceSensor(1);	// Capteur distance 2
	s_cmd_answer->reponse[5] = (int32_t)F_GetDistanceSensor(2);	// Capteur distance 3
	s_cmd_answer->reponse[6] = (int32_t)F_GetDistanceSensor(3);	// Capteur distance 4
	s_cmd_answer->reponse[7] = (int32_t)F_GPIO_GetIOValues();	// Etat LED + Capteurs(Tout ou rien + tirette)
	s_cmd_answer->reponse[8] = (int32_t)0;	// Etat servo

	return status;

}


uint8_t F_Cmd_Set_LED(Tcp_command s_cmd_received, Tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	// RED LED
	if(((s_cmd_received.params[0]>>16)& 0x00FF) == 1)
	{
		F_GPIO_setLedRed(1);
		F_UART_SetReceivedChar('z');
	}
	else if(((s_cmd_received.params[0]>>16)& 0x00FF) == 0)
	{
		F_GPIO_setLedRed(0);
		if(F_UART_GetReceivedChar() == 'z')
		{
			F_UART_SetReceivedChar('a');
		}

	}

	// BLUE LED
	if(((s_cmd_received.params[0]>>8)& 0x00FF) == 1)
	{
		F_GPIO_setLedBlue(1);
		F_UART_SetReceivedChar('s');
	}
	else if(((s_cmd_received.params[0]>>8)& 0x00FF) == 0)
	{
		F_GPIO_setLedBlue(0);
		F_UART_SetReceivedChar('a');
		if(F_UART_GetReceivedChar() == 's')
		{
			F_UART_SetReceivedChar('a');
		}
	}

	// GREEN LED
	if((s_cmd_received.params[0] & 0x00FF) == 1)
	{
		F_GPIO_setLedGreen(1);
		F_UART_SetReceivedChar('q');
	}
	else if((s_cmd_received.params[0]& 0x00FF) == 0)
	{
		F_GPIO_setLedGreen(0);
		F_UART_SetReceivedChar('a');
		if(F_UART_GetReceivedChar() == 'q')
		{
			F_UART_SetReceivedChar('a');
		}
	}

	s_cmd_answer->code_retour = status;
	s_cmd_answer->nb_reponse = 0;

	return status;

}



uint8_t F_Cmd_Manual_Control(Tcp_command s_cmd_received, Tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

	// STOP
	if(s_cmd_received.params[0] == 0)
	{
		F_GPIO_setLedRed(0);
		F_GPIO_setLedBlue(0);
		F_GPIO_setLedGreen(0);
		F_UART_SetReceivedChar('a');
	}
	// MOVE FORWARD
	else if(s_cmd_received.params[0] == 1)
	{
		F_GPIO_setLedRed(1);
		F_GPIO_setLedBlue(0);
		F_GPIO_setLedGreen(0);
		F_UART_SetReceivedChar('z');
	}
	// MOVE BACKWARD
	else if(s_cmd_received.params[0] == 2)
	{
		F_GPIO_setLedRed(0);
		F_GPIO_setLedBlue(1);
		F_GPIO_setLedGreen(0);
		F_UART_SetReceivedChar('s');
	}
	// MOVE LEFT
	else if(s_cmd_received.params[0] == 3)
	{
		F_GPIO_setLedRed(0);
		F_GPIO_setLedBlue(0);
		F_GPIO_setLedGreen(1);
		F_UART_SetReceivedChar('d');
	}
	// MOVE RIGHT
	else if(s_cmd_received.params[0] == 4)
	{
		F_GPIO_setLedRed(0);
		F_GPIO_setLedBlue(0);
		F_GPIO_setLedGreen(1);
		F_UART_SetReceivedChar('q');
	}

	s_cmd_answer->code_retour = status;
	s_cmd_answer->nb_reponse = 0;

	return status;

}

uint8_t F_Cmd_Get_UID(Tcp_command s_cmd_received, Tcp_answer *s_cmd_answer)
{
	uint8_t status = STATUS_OK;

    uint32_t idPart1 = STM32_UUID[0];
    uint32_t idPart2 = STM32_UUID[1];
    uint32_t idPart3 = STM32_UUID[2];

	s_cmd_answer->code_retour = status;
	s_cmd_answer->nb_reponse = 3;
	s_cmd_answer->reponse[0] = idPart1;	// UID(31:0)
	s_cmd_answer->reponse[1] = idPart2;	// UID(63:32)
	s_cmd_answer->reponse[2] = idPart3; // UID(95:64)


	return status;

}


void F_TCP_paquetTocmd(void *data,uint16_t data_len, struct tcp_command *s_cmd_received)
{
	uint8_t i = 0;
	uint32_t * tempPtr = data;

	volatile uint32_t tab_reveived[3 + SIZE_PARAM_CMD] = {0};

	for(i=0; i<(data_len/4);i++)
	{

		tab_reveived[i] = (uint32_t)(*tempPtr++);

	}

	s_cmd_received->id = (uint32_t)(tab_reveived[0]);
	s_cmd_received->cmd = tab_reveived[1];
	s_cmd_received->nb_param = tab_reveived[2];
	for(i=0;i<((data_len/4)-3);i++)
	{
		s_cmd_received->params[i] = (uint32_t)(tab_reveived[3+i]);
	}

}

void F_TCP_answerTotab(uint32_t *array, struct tcp_answer s_cmd_answer)
{
	uint8_t i = 0;
	array[0] = s_cmd_answer.id;
	array[1] = s_cmd_answer.code_retour;
	array[2] = s_cmd_answer.cmd;
	array[3] =  s_cmd_answer.nb_reponse;

	for(i=0;i< s_cmd_answer.nb_reponse;i++)
	{
		array[4+i] = s_cmd_answer.reponse[i];
	}

}

