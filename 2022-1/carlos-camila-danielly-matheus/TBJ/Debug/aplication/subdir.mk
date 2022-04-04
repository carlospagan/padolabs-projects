################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../aplication/app.c 

OBJS += \
./aplication/app.o 

C_DEPS += \
./aplication/app.d 


# Each subdirectory must supply rules for building sources it contributes
aplication/%.o: ../aplication/%.c aplication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"/home/gabrielly/Downloads/TBJ/libs/nrf24" -I"/home/gabrielly/Downloads/TBJ/aplication" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-aplication

clean-aplication:
	-$(RM) ./aplication/app.d ./aplication/app.o

.PHONY: clean-aplication

