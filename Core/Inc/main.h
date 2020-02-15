/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_dma.h"

#include "stm32f4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "gpio.h"
#include "VL53L1X_api.h"
#include "F_VL53L1X.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


#define DUREE_MATCH 100
#define DISTANCE_WARNING 300

#define mainHOST_NAME				"STM32_Robot"
#define mainDEVICE_NICK_NAME		"windows_STM32_Robot"
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define M1_FB_Pin GPIO_PIN_3
#define M1_FB_GPIO_Port GPIOF
#define XSHUNT_5_Pin GPIO_PIN_4
#define XSHUNT_5_GPIO_Port GPIOF
#define XSHUNT_4_Pin GPIO_PIN_5
#define XSHUNT_4_GPIO_Port GPIOF
#define XSHUNT_1_Pin GPIO_PIN_10
#define XSHUNT_1_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define LED_3_Pin GPIO_PIN_3
#define LED_3_GPIO_Port GPIOA
#define TIRETTE_Pin GPIO_PIN_5
#define TIRETTE_GPIO_Port GPIOA
#define M_SF_Pin GPIO_PIN_6
#define M_SF_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define M2_FB_Pin GPIO_PIN_1
#define M2_FB_GPIO_Port GPIOB
#define M2_DIR_Pin GPIO_PIN_12
#define M2_DIR_GPIO_Port GPIOF
#define M1_DIR_Pin GPIO_PIN_13
#define M1_DIR_GPIO_Port GPIOF
#define ENABLE_MOTORS_Pin GPIO_PIN_14
#define ENABLE_MOTORS_GPIO_Port GPIOF
#define CONTACT_4_Pin GPIO_PIN_0
#define CONTACT_4_GPIO_Port GPIOG
#define XSHUNT_2_Pin GPIO_PIN_7
#define XSHUNT_2_GPIO_Port GPIOE
#define XSHUNT_6_Pin GPIO_PIN_8
#define XSHUNT_6_GPIO_Port GPIOE
#define PWM_SERVO_5_Pin GPIO_PIN_9
#define PWM_SERVO_5_GPIO_Port GPIOE
#define LED_2_Pin GPIO_PIN_10
#define LED_2_GPIO_Port GPIOE
#define PWM_SERVO_6_Pin GPIO_PIN_11
#define PWM_SERVO_6_GPIO_Port GPIOE
#define PWM_SERVO_7_Pin GPIO_PIN_13
#define PWM_SERVO_7_GPIO_Port GPIOE
#define PWM_SERVO_8_Pin GPIO_PIN_14
#define PWM_SERVO_8_GPIO_Port GPIOE
#define LED_4_Pin GPIO_PIN_10
#define LED_4_GPIO_Port GPIOB
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define LED_5_Pin GPIO_PIN_13
#define LED_5_GPIO_Port GPIOD
#define CMD_MOTEUR_DROIT_Pin GPIO_PIN_14
#define CMD_MOTEUR_DROIT_GPIO_Port GPIOD
#define CMD_MOTEUR_GAUCHE_Pin GPIO_PIN_15
#define CMD_MOTEUR_GAUCHE_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define XSHUNT_3_Pin GPIO_PIN_8
#define XSHUNT_3_GPIO_Port GPIOG
#define PWM_SERVO_1_Pin GPIO_PIN_6
#define PWM_SERVO_1_GPIO_Port GPIOC
#define PWM_SERVO_2_Pin GPIO_PIN_7
#define PWM_SERVO_2_GPIO_Port GPIOC
#define PWM_SERVO_3_Pin GPIO_PIN_8
#define PWM_SERVO_3_GPIO_Port GPIOC
#define PWM_SERVO_4_Pin GPIO_PIN_9
#define PWM_SERVO_4_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define QEI_M1_CH1_Pin GPIO_PIN_15
#define QEI_M1_CH1_GPIO_Port GPIOA
#define CONTACT_5_Pin GPIO_PIN_0
#define CONTACT_5_GPIO_Port GPIOD
#define CONTACT_1_Pin GPIO_PIN_4
#define CONTACT_1_GPIO_Port GPIOD
#define CONTACT_2_Pin GPIO_PIN_5
#define CONTACT_2_GPIO_Port GPIOD
#define CONTACT_3_Pin GPIO_PIN_7
#define CONTACT_3_GPIO_Port GPIOD
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define QEI_M1_CH2_Pin GPIO_PIN_3
#define QEI_M1_CH2_GPIO_Port GPIOB
#define QEI_M2_CH1_Pin GPIO_PIN_4
#define QEI_M2_CH1_GPIO_Port GPIOB
#define QEI_M2_CH2_Pin GPIO_PIN_5
#define QEI_M2_CH2_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_6
#define LED_1_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define CONTACT_6_Pin GPIO_PIN_1
#define CONTACT_6_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
