################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app.c \
../src/app_mem_managment.c \
../src/cr_startup_lpc17.c \
../src/helper.c \
../src/main.c \
../src/network.c \
../src/printf-stdarg.c 

OBJS += \
./src/app.o \
./src/app_mem_managment.o \
./src/cr_startup_lpc17.o \
./src/helper.o \
./src/main.o \
./src/network.o \
./src/printf-stdarg.o 

C_DEPS += \
./src/app.d \
./src/app_mem_managment.d \
./src/cr_startup_lpc17.d \
./src/helper.d \
./src/main.d \
./src/network.d \
./src/printf-stdarg.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__NEWLIB__ -I"C:\SE\FreeRTOS10-template\inc" -I"C:\SE\MFRC522\inc" -I"C:\SE\SE1819\inc" -I"C:\SE\CMSIS_CORE_LPC17xx\inc" -I"C:\SE\FreeRTOS10_Library\include" -I"C:\SE\FreeRTOS10_Library\src\portable" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


