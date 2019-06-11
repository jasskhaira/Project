################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/pwm\ and\ usart\ rtos.c \
../source/semihost_hardfault.c 

OBJS += \
./source/pwm\ and\ usart\ rtos.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/pwm\ and\ usart\ rtos.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/pwm\ and\ usart\ rtos.o: ../source/pwm\ and\ usart\ rtos.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__LPC5411X__ -DCORE_M4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DCPU_LPC54114J256BD64 -DCPU_LPC54114J256BD64_cm4 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -D__MULTICORE_MASTER -I../board -I../source -I../ -I../amazon-freertos/include -I../amazon-freertos/FreeRTOS/portable -I../drivers -I../CMSIS -I../device -I../drivers/freertos -I../component/serial_manager -I../component/uart -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"source/pwm and usart rtos.d" -MT"source/pwm\ and\ usart\ rtos.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__LPC5411X__ -DCORE_M4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DCPU_LPC54114J256BD64 -DCPU_LPC54114J256BD64_cm4 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -D__MULTICORE_MASTER -I../board -I../source -I../ -I../amazon-freertos/include -I../amazon-freertos/FreeRTOS/portable -I../drivers -I../CMSIS -I../device -I../drivers/freertos -I../component/serial_manager -I../component/uart -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


