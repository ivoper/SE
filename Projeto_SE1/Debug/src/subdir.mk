################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Projeto_SE1.c \
../src/app_mem_managment.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/helper.c 

OBJS += \
./src/Projeto_SE1.o \
./src/app_mem_managment.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/helper.o 

C_DEPS += \
./src/Projeto_SE1.d \
./src/app_mem_managment.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\SE\Projeto_SE1\inc" -I"C:\SE\MFRC522\inc" -I"C:\SE\SE1819\inc" -I"C:\SE\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


