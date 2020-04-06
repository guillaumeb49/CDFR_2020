/**
  ******************************************************************************
  * File Name          : I2C.c
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

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{
  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration  
  PB8   ------> I2C1_SCL
  PB9   ------> I2C1_SDA 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C1 interrupt Init */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);
  NVIC_SetPriority(I2C1_ER_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(I2C1_ER_IRQn);

  /** I2C Initialization 
  */
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 100000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0);

}
/* I2C2 init function */
void MX_I2C2_Init(void)
{
  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  /**I2C2 GPIO Configuration  
  PF0   ------> I2C2_SDA
  PF1   ------> I2C2_SCL 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  /** I2C Initialization 
  */
  LL_I2C_DisableOwnAddress2(I2C2);
  LL_I2C_DisableGeneralCall(I2C2);
  LL_I2C_EnableClockStretching(I2C2);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 100000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C2, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C2, 0);

}

/* USER CODE BEGIN 1 */

/**
 * Write a single value in a slave register
 */
int F_I2C1_WriteValue(uint8_t slave_addr, uint8_t value){

	__disable_irq();

	int i2c_status = I2C_STATUS_OK;
	uint32_t timeout = 0;
//	uint16_t i = 0;

	// Send start
	I2C1->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C1->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		if(timeout > I2C_TIMEOUT){
			//printf("Erreur : Start Condition \n");
			return I2C_STATUS_KO;
		}
	timeout++;
	}
	timeout=0;
	// Send slave address
	I2C1->DR = (slave_addr<<1) & 0xFE  ;	// address + write
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		if(timeout > I2C_TIMEOUT){
			//printf("Erreur : Send slave address \n");
			return I2C_STATUS_KO;
		}
		timeout++;
	}
	timeout=0;
	i2c_status = I2C1->SR2; // read status to clear flag

	// Send register address
	I2C1->DR = value;
	while ((!(I2C1->SR1 & I2C_SR1_TXE)) && (!(I2C1->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)


	I2C1->CR1 |= I2C_CR1_STOP; // send STOP bit

	__enable_irq();

	return i2c_status;
}



/** I2C2 *****/
/**
 * Write a single value in a slave register
 */
uint8_t F_I2C2_WriteRegister(uint8_t slave_addr, uint16_t register_addr, uint8_t *value, uint8_t size){
	uint8_t i2c_status = I2C_STATUS_OK;
	uint32_t timeout = 0;
    uint8_t i = 0;

    // Send start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
	timeout++;
	}
	// Send slave address
	I2C2->DR = slave_addr & 0xFE  ;	// address + write
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}

	i2c_status = I2C2->SR2; // read status to clear flag

	// Send register address MSB
	I2C2->DR = (uint8_t)((register_addr>>8) & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	// Send register address LSB
	I2C2->DR = (uint8_t)(register_addr & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	for(i=0;i<size;i++)
	{

		// Send new value to write to the register
		I2C2->DR = value[i];
		while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	}

	I2C2->CR1 |= I2C_CR1_STOP; // send STOP bit
	return i2c_status;
}

/**
 * Read a single value from a slave register
 */
int F_I2C2_ReadRegister(uint8_t slave_addr, uint8_t register_addr,uint8_t register_addr_2, uint8_t *value_read){
	int i2c_status = I2C_STATUS_OK;	// Init return value to error
	int timeout=0;

	// Send start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
	timeout++;
	}
	// Send slave address
	I2C2->DR = slave_addr & 0xFE  ;	// address + write
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}

	i2c_status = I2C2->SR2; // read status to clear flag

	// Send register address
	I2C2->DR = register_addr;
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	// Send register address
	I2C2->DR = register_addr_2;
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)


	// Send repeated start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB));	// wait for START condition (SB=1)

	timeout=0;
	// Send slave address
	I2C2->DR = slave_addr | 1;	// address + read
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){ // wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}
	i2c_status = I2C2->SR2; // read status to clear flag

	// prepare NACK
	I2C2->CR1 &= ~I2C_CR1_ACK;

	// Wait for Data available
	while (!(I2C2->SR1 & I2C_SR1_RXNE));
	*value_read = I2C2->DR; 			// Address in chip -> DR & write

	// send STOP bit
	I2C2->CR1 |= I2C_CR1_STOP;
	return i2c_status;
}


// Read x value from the device
uint8_t F_I2C2_ReadRegisterVL53L1X(uint8_t slave_addr, uint16_t register_addr, uint8_t nb_value_to_read, uint8_t *value_read){
	uint8_t i2c_status = 0;	// Init return value to error
	uint8_t status = I2C_STATUS_OK;
	uint32_t timeout=0;

	uint8_t i = 0;

	// Send start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_TIMEOUT;
		}
		timeout++;
	}

	// Reset Timeout
	timeout = 0;

	// Send slave address
	I2C2->DR = slave_addr & 0xFE  ;	// address + write
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_TIMEOUT;
		}
		timeout++;
	}

	i2c_status = I2C2->SR2; // read status to clear flag

	// Reset Timeout
	timeout = 0;

	// Send register address MSB
	I2C2->DR = (uint8_t)((register_addr>>8) & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))) // wait for DR empty (TxE)
	{
		if(timeout > I2C_TIMEOUT)
		{
			return I2C_STATUS_TIMEOUT;
		}
		timeout++;
	}

	// Reset Timeout
		timeout = 0;

	// Send register address LSB
	I2C2->DR = (uint8_t)(register_addr & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))) // wait for DR empty (TxE)
	{
		if(timeout > I2C_TIMEOUT)
		{
			return I2C_STATUS_TIMEOUT;
		}
		timeout++;
		}
	// Reset Timeout
	timeout = 0;

	// Send repeated start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB))	// wait for START condition (SB=1)
	{
		if(timeout > I2C_TIMEOUT)
		{
			return I2C_STATUS_TIMEOUT;
		}
		timeout++;
	}

	timeout=0;
	// Send slave address
	I2C2->DR = slave_addr | 1;	// address + read
	while (!(I2C2->SR1 & I2C_SR1_ADDR)) // wait for ADDRESS sent (ADDR=1)
	{
		if(timeout > I2C_TIMEOUT)
		{
			return I2C_STATUS_TIMEOUT;
		}
		timeout++;
	}
	i2c_status = I2C2->SR2; // read status to clear flag


	// prepare ACK
	I2C2->CR1 |= I2C_CR1_ACK;

	for(i=0;i<nb_value_to_read;i++)
	{

		// If this is the last byte to receive
		if((i+1) >= nb_value_to_read)
		{
			// prepare NACK
			I2C2->CR1 &= ~I2C_CR1_ACK;
		}

		timeout=0;

		// Wait for Data available
		while (!(I2C2->SR1 & I2C_SR1_RXNE))
		{
			if(timeout > I2C_TIMEOUT)
			{
				return I2C_STATUS_TIMEOUT;
			}
			timeout++;
		}
		value_read[i] = I2C2->DR; 			// Address in chip -> DR & write
	}


	// send STOP bit
	I2C2->CR1 |= I2C_CR1_STOP;
	return status;
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
