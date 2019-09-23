################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../My_include/src/chirp.c \
../My_include/src/debug.c \
../My_include/src/debug_frmwrk.c \
../My_include/src/exec_m0.c \
../My_include/src/fpu_init.c \
../My_include/src/frame_m0.c \
../My_include/src/ipc_mbx.c \
../My_include/src/lpc43xx_adc.c \
../My_include/src/lpc43xx_cgu.c \
../My_include/src/lpc43xx_i2c.c \
../My_include/src/lpc43xx_scu.c \
../My_include/src/lpc43xx_ssp.c \
../My_include/src/lpc43xx_timer.c \
../My_include/src/lpc43xx_uart.c \
../My_include/src/platform_config.c \
../My_include/src/qqueue.c \
../My_include/src/rls_m0.c \
../My_include/src/smlink.c \
../My_include/src/spifilib_dev_common.c \
../My_include/src/spifilib_fam_standard_cmd.c \
../My_include/src/system_LPC43xx.c \
../My_include/src/usbcore.c \
../My_include/src/usbdesc.c \
../My_include/src/usbhw.c \
../My_include/src/usbuser.c 

CPP_SRCS += \
../My_include/src/blob.cpp \
../My_include/src/blobs.cpp \
../My_include/src/calc.cpp \
../My_include/src/camera.cpp \
../My_include/src/chirp.cpp \
../My_include/src/colorlut.cpp \
../My_include/src/exec.cpp \
../My_include/src/flash.cpp \
../My_include/src/led.cpp \
../My_include/src/libpixyusb2.cpp \
../My_include/src/misc.cpp \
../My_include/src/param.cpp \
../My_include/src/pixy_init.cpp \
../My_include/src/power.cpp \
../My_include/src/qqueue.cpp \
../My_include/src/rcservo.cpp \
../My_include/src/sccb.cpp \
../My_include/src/smlink.cpp \
../My_include/src/usblink.cpp \
../My_include/src/util.cpp 

OBJS += \
./My_include/src/blob.o \
./My_include/src/blobs.o \
./My_include/src/calc.o \
./My_include/src/camera.o \
./My_include/src/chirp.o \
./My_include/src/colorlut.o \
./My_include/src/debug.o \
./My_include/src/debug_frmwrk.o \
./My_include/src/exec.o \
./My_include/src/exec_m0.o \
./My_include/src/flash.o \
./My_include/src/fpu_init.o \
./My_include/src/frame_m0.o \
./My_include/src/ipc_mbx.o \
./My_include/src/led.o \
./My_include/src/libpixyusb2.o \
./My_include/src/lpc43xx_adc.o \
./My_include/src/lpc43xx_cgu.o \
./My_include/src/lpc43xx_i2c.o \
./My_include/src/lpc43xx_scu.o \
./My_include/src/lpc43xx_ssp.o \
./My_include/src/lpc43xx_timer.o \
./My_include/src/lpc43xx_uart.o \
./My_include/src/misc.o \
./My_include/src/param.o \
./My_include/src/pixy_init.o \
./My_include/src/platform_config.o \
./My_include/src/power.o \
./My_include/src/qqueue.o \
./My_include/src/rcservo.o \
./My_include/src/rls_m0.o \
./My_include/src/sccb.o \
./My_include/src/smlink.o \
./My_include/src/spifilib_dev_common.o \
./My_include/src/spifilib_fam_standard_cmd.o \
./My_include/src/system_LPC43xx.o \
./My_include/src/usbcore.o \
./My_include/src/usbdesc.o \
./My_include/src/usbhw.o \
./My_include/src/usblink.o \
./My_include/src/usbuser.o \
./My_include/src/util.o 

C_DEPS += \
./My_include/src/chirp.d \
./My_include/src/debug.d \
./My_include/src/debug_frmwrk.d \
./My_include/src/exec_m0.d \
./My_include/src/fpu_init.d \
./My_include/src/frame_m0.d \
./My_include/src/ipc_mbx.d \
./My_include/src/lpc43xx_adc.d \
./My_include/src/lpc43xx_cgu.d \
./My_include/src/lpc43xx_i2c.d \
./My_include/src/lpc43xx_scu.d \
./My_include/src/lpc43xx_ssp.d \
./My_include/src/lpc43xx_timer.d \
./My_include/src/lpc43xx_uart.d \
./My_include/src/platform_config.d \
./My_include/src/qqueue.d \
./My_include/src/rls_m0.d \
./My_include/src/smlink.d \
./My_include/src/spifilib_dev_common.d \
./My_include/src/spifilib_fam_standard_cmd.d \
./My_include/src/system_LPC43xx.d \
./My_include/src/usbcore.d \
./My_include/src/usbdesc.d \
./My_include/src/usbhw.d \
./My_include/src/usbuser.d 

CPP_DEPS += \
./My_include/src/blob.d \
./My_include/src/blobs.d \
./My_include/src/calc.d \
./My_include/src/camera.d \
./My_include/src/chirp.d \
./My_include/src/colorlut.d \
./My_include/src/exec.d \
./My_include/src/flash.d \
./My_include/src/led.d \
./My_include/src/libpixyusb2.d \
./My_include/src/misc.d \
./My_include/src/param.d \
./My_include/src/pixy_init.d \
./My_include/src/power.d \
./My_include/src/qqueue.d \
./My_include/src/rcservo.d \
./My_include/src/sccb.d \
./My_include/src/smlink.d \
./My_include/src/usblink.d \
./My_include/src/util.d 


# Each subdirectory must supply rules for building sources it contributes
My_include/src/%.o: ../My_include/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"/home/jass/Project/eclipse/bb/My_include" -I"/home/jass/Project/eclipse/bb/My_include/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

My_include/src/%.o: ../My_include/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/home/jass/Project/eclipse/bb/My_include" -I"/home/jass/Project/eclipse/bb/My_include/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


