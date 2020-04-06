
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>


#include "i2c.h"

int8_t VL53L1_WrByte(VL53L1_DEV Dev, uint16_t index, uint8_t data) {

	uint8_t status = I2C_STATUS_OK;
	status = F_I2C2_WriteRegister(Dev->addr, index, &data, 1);

	return status; // to be implemented
}

int8_t VL53L1_WrWord(VL53L1_DEV Dev, uint16_t index, uint16_t data) {

	uint8_t to_send[2] = {0,0};
	uint8_t status = I2C_STATUS_OK;

	to_send[0] = (uint8_t)((data >>8) & 0x00FF); // MSB
	to_send[1] = (uint8_t)(data & 0x00FF);

	status = F_I2C2_WriteRegister(Dev->addr, index, &to_send[0], 1);
	status = F_I2C2_WriteRegister(Dev->addr, index, &to_send[1], 1);
	return status; // to be implemented
}

int8_t VL53L1_WrDWord(VL53L1_DEV Dev, uint16_t index, uint32_t data) {

	uint8_t to_send[4] = {0,0};
	uint8_t status = I2C_STATUS_OK;

	to_send[0] = (uint8_t)((data >> 24) & 0x000000FF); // MSB
	to_send[1] = (uint8_t)((data >> 16) & 0x000000FF);
	to_send[2] = (uint8_t)((data >> 8 ) & 0x000000FF);
	to_send[3] = (uint8_t)(data & 0x000000FF);

	status = F_I2C2_WriteRegister(Dev->addr, index, &to_send[0], 1);
	status = F_I2C2_WriteRegister(Dev->addr, index, &to_send[1], 1);
	status = F_I2C2_WriteRegister(Dev->addr, index, &to_send[2], 1);
	status = F_I2C2_WriteRegister(Dev->addr, index, &to_send[3], 1);

	return status; // to be implemented
}

int8_t VL53L1_RdByte(VL53L1_DEV Dev, uint16_t index, uint8_t *data) {

	uint8_t status = I2C_STATUS_OK;
	status = F_I2C2_ReadRegisterVL53L1X(Dev->addr, index, 1, data);

	return status;
}

int8_t VL53L1_RdWord(VL53L1_DEV Dev, uint16_t index, uint16_t *data) {

	uint8_t read_values[2] = {0,0};
	uint8_t status = I2C_STATUS_OK;

	status = F_I2C2_ReadRegisterVL53L1X(Dev->addr, index, 2, read_values);

	*data = (read_values[0] << 8) + read_values[1];

	return status; // to be implemented
}

int8_t VL53L1_RdDWord(VL53L1_DEV Dev, uint16_t index, uint32_t *data) {

	uint8_t read_values[4] = {0,0,0,0};
	uint8_t status = I2C_STATUS_OK;

	status = F_I2C2_ReadRegisterVL53L1X(Dev->addr, index, 4, read_values);

	*data = (read_values[0] << 24) + (read_values[1] << 16) + (read_values[2] << 8) + (read_values[3]);

	return status; // to be implemented
}

int8_t VL53L1_WaitMs(VL53L1_Dev_t *pdev, int32_t wait_ms){

	HAL_Delay(wait_ms);

	return I2C_STATUS_OK; // to be implemented
}
