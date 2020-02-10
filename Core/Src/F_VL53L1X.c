/*
 * F_VL53L1X.c
 *
 *  Created on: Apr 28, 2019
 *      Author: guillaume
 */

#include "F_VL53L1X.h"

VL53L1_Dev_t dev_avant_1;
VL53L1_Dev_t dev_avant_2;
VL53L1_Dev_t dev_avant_3;
VL53L1_Dev_t dev_arriere_1;

uint16_t distance_avant_1 = 0;
uint16_t distance_avant_2 = 0;
uint16_t distance_avant_3 = 0;
uint16_t distance_arriere_1 = 0;

extern int g_enableAUTO;
extern int g_obstacle_not;
/**
 * Init all the VL53L1X connected to I2C2
 */
void F_VL53L1X_InitSensors()
{
	uint8_t state = 2;
	uint32_t i = 0;

	// Set up I2C initial address
	dev_avant_1.addr = 0x52;
	dev_avant_2.addr = 0x52;
	dev_avant_3.addr = 0x52;
	//dev_arriere_1.addr = 0x52;


	// Configure VL53L1X_AVANT_1_ADDR	0x10	PF2

		// Set PF2 high
		GPIOF->ODR |= GPIO_ODR_OD2;

		// small delay
		for(i=0;i<65000;i++){__asm("NOP");}

		// Wait for device booted
		while((state & 0x01) != 0x01){ VL53L1X_BootState(dev_avant_1, &state); HAL_Delay(2);}

		VL53L1X_SetI2CAddress(dev_avant_1, VL53L1X_AVANT_1_ADDR);
		dev_avant_1.addr = VL53L1X_AVANT_1_ADDR;

		HAL_Delay(1);

		VL53L1X_SensorInit(dev_avant_1);
		VL53L1X_StartRanging(dev_avant_1);


	// Configure VL53L1X_AVANT_2_ADDR	0x12	PF3
	// Set PF2 high
		GPIOF->ODR |= GPIO_ODR_OD3;

		// small delay
		for(i=0;i<65000;i++){__asm("NOP");}

		// Wait for device booted
		while((state & 0x01) != 0x01){VL53L1X_BootState(dev_avant_2, &state); HAL_Delay(2);}

		VL53L1X_SetI2CAddress(dev_avant_2, VL53L1X_AVANT_2_ADDR);
		dev_avant_2.addr = VL53L1X_AVANT_2_ADDR;

		HAL_Delay(1);

		VL53L1X_SensorInit(dev_avant_2);
		VL53L1X_StartRanging(dev_avant_2);


	// Configure VL53L1X_AVANT_3_ADDR	0x14	PF5
		// Set PF2 high
		GPIOF->ODR |= GPIO_ODR_OD5;

		// small delay
		for(i=0;i<65000;i++){__asm("NOP");}

		// Wait for device booted
		while((state & 0x01) != 0x01){VL53L1X_BootState(dev_avant_3, &state); HAL_Delay(2);}

		VL53L1X_SetI2CAddress(dev_avant_3, VL53L1X_AVANT_3_ADDR);
		dev_avant_3.addr = VL53L1X_AVANT_3_ADDR;

		HAL_Delay(1);

		VL53L1X_SensorInit(dev_avant_3);
		VL53L1X_StartRanging(dev_avant_3);


/*	// Configure VL53L1X_ARRIERE_1_ADDR	0x16	PF5
		// Set PF2 high
		GPIOF->ODR |= GPIO_ODR_OD2;

		// small delay
		for(i=0;i<65000;i++){__asm("NOP");}

		// Wait for device booted
		while((state & 0x01) != 0x01){VL53L1X_BootState(dev_arriere_1, &state); HAL_Delay(2);}

		VL53L1X_SetI2CAddress(dev_arriere_1, VL53L1X_ARRIERE_1_ADDR);
		dev_arriere_1.addr = VL53L1X_ARRIERE_1_ADDR;

		HAL_Delay(1);

		VL53L1X_SensorInit(dev_arriere_1);
		VL53L1X_StartRanging(dev_arriere_1);*/
}

/**
 * Check the VL53L1X connected to the STM32
 * If data is ready ,update the value of distance_avant_1, distance_avant_2, distance_avant_3, distance_arriere_1
 */
void F_VL53L1X_CheckSensors(void)
{
	uint8_t dataReady = 0;
	uint8_t rangeStatus = 0;

	// dev_avant_1;
	VL53L1X_CheckForDataReady(dev_avant_1, &dataReady);

	if(dataReady  != 0)
	{
		VL53L1X_GetRangeStatus(dev_avant_1, &rangeStatus);
		if(rangeStatus == 0)
		{
			VL53L1X_GetDistance(dev_avant_1, &distance_avant_1);
		}
		else
		{
			distance_avant_1 = 1000;
		}
		VL53L1X_ClearInterrupt(dev_avant_1);

	}

	// dev_avant_2;
	VL53L1X_CheckForDataReady(dev_avant_2, &dataReady);

	if(dataReady  != 0)
	{
		VL53L1X_GetRangeStatus(dev_avant_2, &rangeStatus);
		if(rangeStatus == 0)
		{
			VL53L1X_GetDistance(dev_avant_2, &distance_avant_2);
		}
		else
		{
			distance_avant_2 = 1000;
		}
		VL53L1X_ClearInterrupt(dev_avant_2);
	}

	// dev_avant_3;
	VL53L1X_CheckForDataReady(dev_avant_3, &dataReady);

	if(dataReady  != 0)
	{
		VL53L1X_GetRangeStatus(dev_avant_3, &rangeStatus);
		if(rangeStatus == 0)
		{
			VL53L1X_GetDistance(dev_avant_3, &distance_avant_3);
		}
		else
		{
			distance_avant_3 = 1000;
		}
		VL53L1X_ClearInterrupt(dev_avant_3);
	}


	// dev_arriere_1;
	/*VL53L1X_CheckForDataReady(dev_arriere_1, &dataReady);

	if(dataReady  != 0)
	{
		VL53L1X_GetRangeStatus(dev_arriere_1, &rangeStatus);
		if(rangeStatus == 0)
		{
			VL53L1X_GetDistance(dev_arriere_1, &distance_arriere_1);
		}
		VL53L1X_ClearInterrupt(dev_arriere_1);
	}*/



	if(((distance_avant_1 < 100) || (distance_avant_2 < 100) || (distance_avant_3 < 100)))
	{
		g_obstacle_not = 0;
	}
	else
	{
		g_obstacle_not = 1;
	}


}




