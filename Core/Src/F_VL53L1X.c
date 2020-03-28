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

extern int g_enableAUTO;
extern int g_obstacle_not;
/**
 * Init all the VL53L1X connected to I2C2
 */
void F_VL53L1X_InitSensors()
{
	// turn led off
	F_GPIO_SetLed1(1);
	F_GPIO_SetLed2(1);
	F_GPIO_SetLed3(1);
	F_GPIO_SetLed4(1);

	// Set-up initial values VL53L1X

	dev_avant_1.sensor_id = 1;
	dev_avant_2.sensor_id = 2;
	dev_avant_3.sensor_id = 3;
	dev_arriere_1.sensor_id = 4;

	dev_avant_1.connected = 0;
	dev_avant_2.connected = 0;
	dev_avant_3.connected = 0;
	dev_arriere_1.connected = 0;

	dev_avant_1.last_distance = 0;
	dev_avant_2.last_distance = 0;
	dev_avant_3.last_distance = 0;
	dev_arriere_1.last_distance = 0;

	// Set up I2C initial address
	dev_avant_1.addr = 0x52;
	dev_avant_2.addr = 0x52;
	dev_avant_3.addr = 0x52;
	dev_arriere_1.addr = 0x52;


	F_VL53L1X_PerformReboot(&dev_avant_1);
	F_VL53L1X_PerformReboot(&dev_avant_2);
	F_VL53L1X_PerformReboot(&dev_avant_3);
	F_VL53L1X_PerformReboot(&dev_arriere_1);


	// Turn LED on to display connection status
	F_GPIO_SetLed1(!dev_avant_1.connected);
	F_GPIO_SetLed2(!dev_avant_2.connected);
	F_GPIO_SetLed3(!dev_avant_3.connected);
	F_GPIO_SetLed4(!dev_arriere_1.connected);

	// Delay to give some time (5s)to see connection status
	osDelay(5000);

	// turn led off
	F_GPIO_SetLed1(1);
	F_GPIO_SetLed2(1);
	F_GPIO_SetLed3(1);
	F_GPIO_SetLed4(1);


}

/**
 * Perform a reboot of the VL53L1X number sensor_nb
 *
 */
uint8_t F_VL53L1X_PerformReboot(VL53L1_Dev_t *sensor)
{
	uint8_t state = 2;
	uint8_t status = 0;
	uint32_t adr = 0;

	switch (sensor->sensor_id)
	{
		case 1:
			// Set PF10 low
			GPIOF->ODR &= ~GPIO_ODR_OD10;
			HAL_Delay(1);
			// Set PF10 high
			GPIOF->ODR |= GPIO_ODR_OD10;

			adr = VL53L1X_AVANT_1_ADDR;
			break;

		case 2:
			// Set PE7 low
			GPIOE->ODR &= ~GPIO_ODR_OD7;
			HAL_Delay(1);
			// Set PE7 high
			GPIOE->ODR |= GPIO_ODR_OD7;

			adr = VL53L1X_AVANT_2_ADDR;
			break;

		case 3:
			// Set PG8 low
			GPIOG->ODR &= ~GPIO_ODR_OD8;
			HAL_Delay(1);
			// Set PG8 high
			GPIOG->ODR |= GPIO_ODR_OD8;

			adr = VL53L1X_AVANT_3_ADDR;
			break;

		case 4:
			// Set PF5 low
			GPIOF->ODR &= ~GPIO_ODR_OD5;
			HAL_Delay(1);
			// Set PF5 high
			GPIOF->ODR |= GPIO_ODR_OD5;

			adr = VL53L1X_ARRIERE_1_ADDR;
			break;
	}

	// Reset original I2C address of this device
	sensor->addr = 0x52;

	// small delay
	HAL_Delay(1);

	// Wait for device booted
	while((state & 0x01) != 0x01)
	{
		status = VL53L1X_BootState(*sensor, &state);

		if(status == I2C_STATUS_TIMEOUT)
		{
			// Sensor is not connected
			sensor->connected = 0;
			return I2C_STATUS_TIMEOUT;
		}

		HAL_Delay(2);
	}

	// Set new I2C address
	status = VL53L1X_SetI2CAddress(*sensor, adr);
	if(status == I2C_STATUS_TIMEOUT)
	{
		// Sensor is not connected
		sensor->connected = 0;
		return I2C_STATUS_TIMEOUT;
	}
	else
	{
		// Sensor is connected
		sensor->connected = 1;
		sensor->addr = adr;
	}

	HAL_Delay(1);

	// Init Sensor for Distance measurement
	VL53L1X_SensorInit(*sensor);

	// Init Sensor for Distance measurement
	VL53L1X_StartRanging(*sensor);

	return 0;
}

/**
 * Check the VL53L1X connected to the STM32
 * If data is ready ,update the value of distance_avant_1, distance_avant_2, distance_avant_3, distance_arriere_1
 */
void F_VL53L1X_CheckSensors(void)
{
	uint8_t dataReady = 0;
	uint8_t rangeStatus = 0;


	// for each connected device check for distance ready to be read
	if(dev_avant_1.connected == 1)
	{
		VL53L1X_CheckForDataReady(dev_avant_1, &dataReady);

		if(dataReady  != 0)
		{
			VL53L1X_GetRangeStatus(dev_avant_1, &rangeStatus);
			if(rangeStatus == 0)
			{
				VL53L1X_GetDistance(dev_avant_1, &dev_avant_1.last_distance);
			}
			else
			{
				dev_avant_1.last_distance= 5000;
			}
			VL53L1X_ClearInterrupt(dev_avant_1);

		}
	}
	if(dev_avant_2.connected == 1)
	{
		VL53L1X_CheckForDataReady(dev_avant_2, &dataReady);

		if(dataReady  != 0)
		{
			VL53L1X_GetRangeStatus(dev_avant_2, &rangeStatus);
			if(rangeStatus == 0)
			{
				VL53L1X_GetDistance(dev_avant_2, &dev_avant_2.last_distance);
			}
			else
			{
				dev_avant_2.last_distance = 5000;
			}
			VL53L1X_ClearInterrupt(dev_avant_2);

		}
	}
	if(dev_avant_3.connected == 1)
	{
		VL53L1X_CheckForDataReady(dev_avant_3, &dataReady);

		if(dataReady  != 0)
		{
			VL53L1X_GetRangeStatus(dev_avant_3, &rangeStatus);
			if(rangeStatus == 0)
			{
				VL53L1X_GetDistance(dev_avant_3, &dev_avant_3.last_distance);
			}
			else
			{
				dev_avant_3.last_distance = 5000;
			}
			VL53L1X_ClearInterrupt(dev_avant_3);

		}
	}
	if(dev_arriere_1.connected == 1)
	{
		VL53L1X_CheckForDataReady(dev_arriere_1, &dataReady);

		if(dataReady  != 0)
		{
			VL53L1X_GetRangeStatus(dev_arriere_1, &rangeStatus);
			if(rangeStatus == 0)
			{
				VL53L1X_GetDistance(dev_arriere_1, &dev_arriere_1.last_distance);
			}
			else
			{
				dev_arriere_1.last_distance = 5000;
			}
			VL53L1X_ClearInterrupt(dev_arriere_1);

		}
	}
}


uint32_t F_GetDistanceSensor(uint8_t capteur_number)
{
	return dev_avant_1.last_distance;
}



