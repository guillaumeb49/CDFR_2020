/*
 * A_com.h
 *
 *  Created on: Feb 9, 2020
 *      Author: guillaume
 */

#ifndef INC_A_COM_H_
#define INC_A_COM_H_

#include "lwip/opt.h"


#include "lwip/sys.h"
#include "lwip/api.h"

#include "gpio.h"

#define TCPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 5 )

#define CMD_INFO			0x01
#define CMD_SET_LED			0x02
#define CMD_GET_LED			0x03

#define CMD_GET_DISTANCES	0x04

#define CMD_SET_POSITION	0x05
#define CMD_GET_POSITION	0x06
#define CMD_GO				0x07
#define CMD_ADD_POINT		0x08
#define CMD_RESET_LIST_WAYPOINTS	0x09
#define CMD_GET_LIST_POINTS	0x0A
#define CMD_GET_TIRETTE     0x0B

#define CMD_MOVE_SERVO     	0x0C

#define STATUS_OK			0x00
#define STATUS_UNKNOWN_CMD	0x01
#define STATUS_ERROR_PARAM	0x02

#define NB_OCTETS_CMD_MAX 14
#define SIZE_PARAM_CMD 4
#define SIZE_ANSWER 10

struct tcp_command
{
	uint32_t id;
	uint32_t cmd;
	uint32_t nb_param;
	uint32_t params[SIZE_PARAM_CMD];
};
typedef struct tcp_command Tcp_command;


/* structure that represents an answer between the STM32 and the Raspberry Pi*/
struct tcp_answer
{
	uint32_t id;
	uint32_t cmd;
	uint32_t code_retour;
	uint32_t nb_reponse;
	uint32_t reponse[SIZE_ANSWER];
};

typedef struct tcp_answer Tcp_answer;

void tcpecho_init(void);

uint8_t F_Cmd_Info(Tcp_command s_cmd_received, Tcp_answer *s_cmd_answer);
uint8_t F_Cmd_Set_LED(Tcp_command s_cmd_received, Tcp_answer *s_cmd_answer);

void F_TCP_paquetTocmd(void *data,uint16_t data_len, struct tcp_command *s_cmd_received);
void F_TCP_answerTotab(uint32_t *array, struct tcp_answer s_cmd_answer);
void F_Process_Command(struct tcp_command s_cmd_received, struct tcp_answer *s_cmd_answer);

#endif /* INC_A_COM_H_ */
