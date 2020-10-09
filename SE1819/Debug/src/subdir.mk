################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/e2prom.c \
../src/esp.c \
../src/flash.c \
../src/i2c.c \
../src/keypad.c \
../src/keypados.c \
../src/lcd.c \
../src/lcdos.c \
../src/led.c \
../src/mfrc.c \
../src/ntp.c \
../src/rb.c \
../src/rtc.c \
../src/spi.c \
../src/uart.c \
../src/wait.c \
../src/waitos.c \
../src/wifi.c 

OBJS += \
./src/e2prom.o \
./src/esp.o \
./src/flash.o \
./src/i2c.o \
./src/keypad.o \
./src/keypados.o \
./src/lcd.o \
./src/lcdos.o \
./src/led.o \
./src/mfrc.o \
./src/ntp.o \
./src/rb.o \
./src/rtc.o \
./src/spi.o \
./src/uart.o \
./src/wait.o \
./src/waitos.o \
./src/wifi.o 

C_DEPS += \
./src/e2prom.d \
./src/esp.d \
./src/flash.d \
./src/i2c.d \
./src/keypad.d \
./src/keypados.d \
./src/lcd.d \
./src/lcdos.d \
./src/led.d \
./src/mfrc.d \
./src/ntp.d \
./src/rb.d \
./src/rtc.d \
./src/spi.d \
./src/uart.d \
./src/wait.d \
./src/waitos.d \
./src/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -I"C:\SE\SE1819\inc" -I"C:\SE\FreeRTOS10_Library\src\portable" -I"C:\SE\FreeRTOS10_Library\include" -I"C:\SE\FreeRTOS10-template\inc" -I"C:\SE\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


