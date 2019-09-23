################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pixy-includes/BBSpi.cpp 

OBJS += \
./pixy-includes/BBSpi.o 

CPP_DEPS += \
./pixy-includes/BBSpi.d 


# Each subdirectory must supply rules for building sources it contributes
pixy-includes/%.o: ../pixy-includes/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"/home/jass/Project/eclipse/PixySpi/pixy-includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


