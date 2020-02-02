################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API_VL53L1_ULD/core/VL53L1X_api.c \
../Drivers/API_VL53L1_ULD/core/VL53L1X_calibration.c 

OBJS += \
./Drivers/API_VL53L1_ULD/core/VL53L1X_api.o \
./Drivers/API_VL53L1_ULD/core/VL53L1X_calibration.o 

C_DEPS += \
./Drivers/API_VL53L1_ULD/core/VL53L1X_api.d \
./Drivers/API_VL53L1_ULD/core/VL53L1X_calibration.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API_VL53L1_ULD/core/VL53L1X_api.o: ../Drivers/API_VL53L1_ULD/core/VL53L1X_api.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/API_VL53L1_ULD/core" -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/API_VL53L1_ULD/platform" -I../Middlewares/Third_Party/LwIP/src/include/posix -I../Middlewares/Third_Party/LwIP/src/include/posix/sys -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../Core/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include/netif -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/API_VL53L1_ULD/core/VL53L1X_api.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/API_VL53L1_ULD/core/VL53L1X_calibration.o: ../Drivers/API_VL53L1_ULD/core/VL53L1X_calibration.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/API_VL53L1_ULD/core" -I"/home/guillaume/STM32CubeIDE/workspace_1.1.0/CDFR_2020/Drivers/API_VL53L1_ULD/platform" -I../Middlewares/Third_Party/LwIP/src/include/posix -I../Middlewares/Third_Party/LwIP/src/include/posix/sys -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../Core/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include/netif -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/API_VL53L1_ULD/core/VL53L1X_calibration.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

