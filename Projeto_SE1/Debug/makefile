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
all: Projeto_SE1.axf

# Tool invocations
Projeto_SE1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\SE\CMSIS_CORE_LPC17xx\Debug" -L"C:\SE\MFRC522\Release" -L"C:\SE\SE1819\Debug" -Xlinker -Map="Projeto_SE1.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "Projeto_SE1_Debug.ld" -o "Projeto_SE1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Projeto_SE1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Projeto_SE1.axf"; # arm-none-eabi-objcopy -v -O binary "Projeto_SE1.axf" "Projeto_SE1.bin" ; # checksum -p LPC1769 -d "Projeto_SE1.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
