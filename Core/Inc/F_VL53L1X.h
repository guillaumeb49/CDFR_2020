#ifndef __F_VL53L1X_H
#define __F_VL53L1X_H

#include "main.h"

#define VL53L1X_AVANT_1_ADDR	0x10
#define VL53L1X_AVANT_2_ADDR	0x12
#define VL53L1X_AVANT_3_ADDR	0x14
#define VL53L1X_ARRIERE_1_ADDR	0x16

extern uint16_t distance_avant_1;
extern uint16_t distance_avant_2;
extern uint16_t distance_avant_3;
extern uint16_t distance_arriere_1;

void F_VL53L1X_InitSensors();
void F_VL53L1X_CheckSensors();
uint32_t F_GetDistanceSensor(uint8_t capteur_number);

#endif
