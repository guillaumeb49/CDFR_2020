/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define LED_RED		GPIO_ODR_ODR_14
#define LED_GREEN	GPIO_ODR_ODR_0
#define LED_BLUE	GPIO_ODR_ODR_7
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void F_GPIO_setLedRed(uint8_t state);
void F_GPIO_setLedBlue(uint8_t state);
void F_GPIO_setLedGreen(uint8_t state);

void F_GPIO_ToogleLedRed(void);
void F_GPIO_PrintCode(uint8_t from0to31);
void F_GPIO_SetLed1(uint8_t boul);
void F_GPIO_SetLed2(uint8_t boul);
void F_GPIO_SetLed3(uint8_t boul);
void F_GPIO_SetLed4(uint8_t boul);
void F_GPIO_SetLed5(uint8_t boul);
void F_GPIO_ToogleLed1(void);
void F_GPIO_ToogleLed2(void);
void F_GPIO_ToogleLed3(void);
void F_GPIO_ToogleLed4(void);
void F_GPIO_ToogleLed5(void);
void F_GPIO_SetEnableMotors(uint8_t);
void F_GPIO_SetMotorDroitDir(int8_t);
void F_GPIO_SetMotorGaucheDir(int8_t);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
