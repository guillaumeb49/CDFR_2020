################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.c 

OBJS += \
./Drivers/FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.o 

C_DEPS += \
./Drivers/FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.o: ../Drivers/FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/FreeRTOS-Plus-TCP/include" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/Inc -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/API_VL53L1_ULD/platform" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/API_VL53L1_ULD/core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

