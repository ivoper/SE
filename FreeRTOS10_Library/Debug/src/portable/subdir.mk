################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/portable/port.c 

OBJS += \
./src/portable/port.o 

C_DEPS += \
./src/portable/port.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/%.o: ../src/portable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -I"C:\SE\FreeRTOS10_Library\include" -I"C:\SE\CMSIS_CORE_LPC17xx\inc" -I"C:\SE\FreeRTOS10_Library\src\portable" -I"C:\SE\FreeRTOS10-template\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


