/**
  ******************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define I2C_TIMEOUT 	100000
#define I2C_STATUS_OK  	0
#define I2C_STATUS_KO 	1

/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */
int F_I2C1_WriteValue(uint8_t slave_addr, uint8_t value);

int F_I2C2_ReadRegister(uint8_t slave_addr, uint8_t register_addr,uint8_t register_addr_2, uint8_t *value_read);
uint8_t F_I2C2_WriteRegister(uint8_t slave_addr, uint16_t register_addr, uint8_t *value, uint8_t size);
uint8_t F_I2C2_ReadRegisterVL53L1X(uint8_t slave_addr, uint16_t register_addr, uint8_t nb_value_to_read, uint8_t *value_read);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
