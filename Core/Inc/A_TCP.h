/*
 * A_TCP.h
 *
 *  Created on: 5 août 2018
 *      Author: guillaume
 */

#ifndef A_TCP_H_
#define A_TCP_H_

#include "tcp_server.h"
#include "main.h"





void F_TCP_paquetTocmd(struct pbuf *p, Tcp_command *s_cmd_received);
void F_TCP_answerTotab(uint8_t *array, Tcp_answer s_cmd_answer);
void F_TCP_cmdTotab(uint8_t *array, Tcp_command s_cmd);
uint8_t F_TCP_send_command(Tcp_command s_cmd);

#endif /* A_TCP_H_ */
