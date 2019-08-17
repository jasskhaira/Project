################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../amazon-freertos/freertos/portable/heap_4.c \
../amazon-freertos/freertos/portable/port.c 

OBJS += \
./amazon-freertos/freertos/portable/heap_4.o \
./amazon-freertos/freertos/portable/port.o 

C_DEPS += \
./amazon-freertos/freertos/portable/heap_4.d \
./amazon-freertos/freertos/portable/port.d 


# Each subdirectory must supply rules for building sources it contributes
amazon-freertos/freertos/portable/%.o: ../amazon-freertos/freertos/portable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__MULTICORE_NONE -DCPU_LPC54114J256BD64 -DCPU_LPC54114J256BD64_cm4 -D__LPC5411X__ -DCORE_M4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__MULTICORE_MASTER -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -I../amazon-freertos/freertos/portable -I../amazon-freertos/include -I../utilities -I../device -I../component/lists -I../drivers -I../CMSIS -I../component/serial_manager -I../source -I../board -I../ -I../startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


