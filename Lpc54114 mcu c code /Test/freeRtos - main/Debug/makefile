################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/freertos/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include board/subdir.mk
-include amazon-freertos/FreeRTOS/portable/subdir.mk
-include amazon-freertos/FreeRTOS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: freeRtos\ -\ main.axf

# Tool invocations
freeRtos\ -\ main.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/jass/Git_data/Project/Lpc54114 mcu c code /Test/freeRtos - main/libs" -Xlinker -Map="freeRtos - main.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T freeRtos_-_main_Debug.ld -o "freeRtos - main.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) "freeRtos - main.axf"
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "freeRtos - main.axf"; # arm-none-eabi-objcopy -v -O binary "freeRtos - main.axf" "freeRtos - main.bin" ; # checksum -p LPC54114J256 -d "freeRtos - main.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
