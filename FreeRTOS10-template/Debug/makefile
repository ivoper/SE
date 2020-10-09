################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
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
all: FreeRTOS10-template.axf

# Tool invocations
FreeRTOS10-template.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\SE\CMSIS_CORE_LPC17xx\Debug" -L"C:\SE\MFRC522\Release" -L"C:\SE\SE1819\Debug" -L"C:\SE\FreeRTOS10_Library\Debug" -Xlinker --gc-sections -Xlinker -Map=FreeRTOS10-template.map -mcpu=cortex-m3 -mthumb -T "FreeRTOS10-template_Debug.ld" -o "FreeRTOS10-template.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) FreeRTOS10-template.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size FreeRTOS10-template.axf; # arm-none-eabi-objcopy -O ihex FreeRTOS10-template.axf FreeRTOS10-template.hex ;
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
