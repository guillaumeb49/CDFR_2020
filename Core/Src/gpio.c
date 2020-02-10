/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PE2   ------> SPI4_SCK
     PE4   ------> SPI4_NSS
     PE5   ------> SPI4_MISO
     PE6   ------> SPI4_MOSI
     PE9   ------> S_TIM1_CH1
     PE11   ------> S_TIM1_CH2
     PE13   ------> S_TIM1_CH3
     PE14   ------> S_TIM1_CH4
     PC6   ------> S_TIM8_CH1
     PC7   ------> S_TIM8_CH2
     PC8   ------> S_TIM8_CH3
     PC9   ------> S_TIM8_CH4
     PG9   ------> USART6_RX
     PG14   ------> USART6_TX
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, XSHUNT_5_Pin|XSHUNT_4_Pin|XSHUNT_1_Pin|M2_DIR_Pin 
                          |M1_DIR_Pin|ENABLE_MOTORS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LED_4_Pin|LD3_Pin|LED_1_Pin 
                          |LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, XSHUNT_2_Pin|XSHUNT_6_Pin|LED_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, USB_PowerSwitchOn_Pin|XSHUNT_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE4 PE5 PE6 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = M1_FB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(M1_FB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin 
                           PFPin PFPin */
  GPIO_InitStruct.Pin = XSHUNT_5_Pin|XSHUNT_4_Pin|XSHUNT_1_Pin|M2_DIR_Pin 
                          |M1_DIR_Pin|ENABLE_MOTORS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = TIRETTE_Pin|M_SF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin */
  GPIO_InitStruct.Pin = LD1_Pin|LED_4_Pin|LD3_Pin|LED_1_Pin 
                          |LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = M2_FB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(M2_FB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin */
  GPIO_InitStruct.Pin = CONTACT_4_Pin|USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = XSHUNT_2_Pin|XSHUNT_6_Pin|LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = PWM_SERVO_5_Pin|PWM_SERVO_6_Pin|PWM_SERVO_7_Pin|PWM_SERVO_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin|XSHUNT_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = PWM_SERVO_1_Pin|PWM_SERVO_2_Pin|PWM_SERVO_3_Pin|PWM_SERVO_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = CONTACT_5_Pin|CONTACT_1_Pin|CONTACT_2_Pin|CONTACT_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PG9 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CONTACT_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CONTACT_6_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void F_GPIO_ToogleLedRed(void){
	if((GPIOB->ODR & LED_Red) > 0){
		GPIOB->ODR &= ~LED_Red;
	}else{
		GPIOB->ODR |= LED_Red;
	}
}
void F_GPIO_PrintCode(int from0to31){
	// TODO
}
void F_GPIO_SetLed1(int boul){
	if(boul != 0){
		LED_1_GPIO_Port->ODR &= ~LED_1_Pin;
	}else{
		LED_1_GPIO_Port->ODR |= LED_1_Pin;
	}
}
void F_GPIO_ToogleLed1(void){
	if((LED_1_GPIO_Port->ODR & LED_1_Pin) > 0){
		LED_1_GPIO_Port->ODR &= ~LED_1_Pin;
	}else{
		LED_1_GPIO_Port->ODR |= LED_1_Pin;
	}
}
void F_GPIO_SetLed2(int boul){
	if(boul != 0){
		LED_4_GPIO_Port->ODR &= ~LED_4_Pin;
	}else{
		LED_4_GPIO_Port->ODR |= LED_4_Pin;
	}
}
void F_GPIO_ToogleLed2(void){
	if((LED_4_GPIO_Port->ODR & LED_4_Pin) > 0){
		LED_4_GPIO_Port->ODR &= ~LED_4_Pin;
	}else{
		LED_4_GPIO_Port->ODR |= LED_4_Pin;
	}
}
void F_GPIO_SetLed3(int boul){
	if(boul != 0){
		LED_3_GPIO_Port->ODR &= ~LED_3_Pin;
	}else{
		LED_3_GPIO_Port->ODR |= LED_3_Pin;
	}
}
void F_GPIO_ToogleLed3(void){
	if((LED_3_GPIO_Port->ODR & LED_3_Pin) > 0){
		LED_3_GPIO_Port->ODR &= ~LED_3_Pin;
	}else{
		LED_3_GPIO_Port->ODR |= LED_3_Pin;
	}
}
void F_GPIO_SetLed4(int boul){
	if(boul != 0){
		LED_5_GPIO_Port->ODR &= ~LED_5_Pin;
	}else{
		LED_5_GPIO_Port->ODR |= LED_5_Pin;
	}
}
void F_GPIO_ToogleLed4(void){
	if((LED_5_GPIO_Port->ODR & LED_5_Pin) > 0){
		LED_5_GPIO_Port->ODR &= ~LED_5_Pin;
	}else{
		LED_5_GPIO_Port->ODR |= LED_5_Pin;
	}
}
void F_GPIO_SetLed5(int boul){
	if(boul != 0){
		LED_2_GPIO_Port->ODR &= ~LED_2_Pin;
	}else{
		LED_2_GPIO_Port->ODR |= LED_2_Pin;
	}
}
void F_GPIO_ToogleLed5(void){
	if((LED_2_GPIO_Port->ODR & LED_2_Pin) > 0){
		LED_2_GPIO_Port->ODR &= ~LED_2_Pin;
	}else{
		LED_2_GPIO_Port->ODR |= LED_2_Pin;
	}
}
void F_GPIO_SetMotorDroitDir(int dir){
	// PF 12
	(dir<0) ? (M1_DIR_GPIO_Port->ODR |= M1_DIR_Pin) : (M1_DIR_GPIO_Port->ODR &= ~M1_DIR_Pin) ;
}

void F_GPIO_SetMotorGaucheDir(int dir){
	// PF 13
	(dir<0) ? (M2_DIR_GPIO_Port->ODR |= M2_DIR_Pin) : (M1_DIR_GPIO_Port->ODR &= ~M2_DIR_Pin) ;
}

void F_GPIO_SetEnableMotors(int state){
	// PF 14
	(state) ? (ENABLE_MOTORS_GPIO_Port->ODR |= ENABLE_MOTORS_Pin) : (ENABLE_MOTORS_GPIO_Port->ODR &= ~ENABLE_MOTORS_Pin) ;
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
