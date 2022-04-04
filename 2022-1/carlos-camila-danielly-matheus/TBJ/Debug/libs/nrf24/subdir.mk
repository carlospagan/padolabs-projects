################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/nrf24/nrf24.c 

OBJS += \
./libs/nrf24/nrf24.o 

C_DEPS += \
./libs/nrf24/nrf24.d 


# Each subdirectory must supply rules for building sources it contributes
libs/nrf24/%.o: ../libs/nrf24/%.c libs/nrf24/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"/home/gabrielly/Downloads/TBJ/libs/nrf24" -I"/home/gabrielly/Downloads/TBJ/aplication" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-libs-2f-nrf24

clean-libs-2f-nrf24:
	-$(RM) ./libs/nrf24/nrf24.d ./libs/nrf24/nrf24.o

.PHONY: clean-libs-2f-nrf24

