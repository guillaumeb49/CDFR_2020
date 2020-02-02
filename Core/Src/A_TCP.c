/*
 * A_TCP.c
 *
 *  Created on: 5 August 2018
 *      Author: guillaume
 */

#include "A_TCP.h"





void F_TCP_paquetTocmd(struct pbuf *p, struct tcp_command *s_cmd_received)
{
	uint8_t *tempPtr;
	uint8_t i = 0;
	tempPtr = (uint8_t *)p->payload;
	volatile uint8_t tab_reveived[20] = {0};

	for(i=0; i<(p->len);i++)
	{

		tab_reveived[i] = (uint8_t) *(tempPtr++);

	}

	s_cmd_received->id = (uint32_t)((tab_reveived[0] << 24) + (tab_reveived[1] << 16) + (tab_reveived[2] << 8) + tab_reveived[3]);
	s_cmd_received->nb_octet = tab_reveived[4];
	s_cmd_received->cmd = tab_reveived[5];
	s_cmd_received->nb_param = tab_reveived[6];
	s_cmd_received->params[0] = (uint16_t)((tab_reveived[7] << 8) + tab_reveived[8]);
	s_cmd_received->params[1] = (uint16_t)((tab_reveived[9] << 8) + tab_reveived[10]);
	s_cmd_received->params[2] = (uint16_t)((tab_reveived[11] << 8) +tab_reveived[12]);
	s_cmd_received->params[3] = (uint16_t)((tab_reveived[13] << 8) + tab_reveived[14]);

}

void F_TCP_answerTotab(uint8_t *array, struct tcp_answer s_cmd_answer)
{
	uint8_t i = 0, j = 0;

	array[0] = (uint8_t)(0x00FF & (s_cmd_answer.id >> 24));
	array[1] = (uint8_t)(0x00FF & (s_cmd_answer.id >> 16));
	array[2] = (uint8_t)(0x00FF & (s_cmd_answer.id >> 8));
	array[3] = (uint8_t)(0x00FF & s_cmd_answer.id);
	array[4] = s_cmd_answer.nb_octet;
	array[5] = s_cmd_answer.cmd;
	array[6] = s_cmd_answer.code_retour;

	for(i=0;i<SIZE_PARAM_CMD;i++)
	{
		array[7+j] = (uint8_t)(0x00FF & (s_cmd_answer.reponse[i] >> 8));
		array[7+j+1] = (uint8_t)(0x00FF & s_cmd_answer.reponse[i]);

		j+=2;
	}


}

void F_TCP_cmdTotab(uint8_t *array, struct tcp_command s_cmd)
{
	uint8_t i = 0, j = 0;

	array[0] = (uint8_t)(0x000F & (s_cmd.id >> 24));
	array[1] = (uint8_t)(0x000F & (s_cmd.id >> 16));
	array[2] = (uint8_t)(0x000F & (s_cmd.id >> 8));
	array[3] = (uint8_t)(0x000F & s_cmd.id);
	array[4] = s_cmd.nb_octet;
	array[5] = s_cmd.cmd;
	array[6] = s_cmd.nb_param;

	for(i=0;i<SIZE_PARAM_CMD;i++)
	{
		array[7+j] = (uint8_t)(0x000F & (s_cmd.params[i] >> 8));
		array[7+j+1] = (uint8_t)(0x000F & s_cmd.params[i]);

		j+=2;
	}


}
