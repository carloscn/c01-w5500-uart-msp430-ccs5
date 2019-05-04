################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./src/spi.obj" \
"./src/rs232.obj" \
"./src/main.obj" \
"./src/W5500.obj" \
"../lnk_msp430f149.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: C01_W5500_UART_MSP430_CCSV5_5.out

# Tool invocations
C01_W5500_UART_MSP430_CCSV5_5.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi --advice:power=all -g --define=__MSP430F149__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal -z -m"C01_W5500_UART_MSP430_CCSV5_5.map" --heap_size=80 --stack_size=80 --use_hw_mpy=16 -i"C:/opt/ti/ccsv5/ccs_base/msp430/include" -i"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/lib" -i"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="C01_W5500_UART_MSP430_CCSV5_5_linkInfo.xml" --rom_model -o "C01_W5500_UART_MSP430_CCSV5_5.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MSP430_EXECUTABLE_OUTPUTS__QUOTED) "C01_W5500_UART_MSP430_CCSV5_5.out"
	-$(RM) "src\W5500.pp" "src\main.pp" "src\rs232.pp" "src\spi.pp" 
	-$(RM) "src\W5500.obj" "src\main.obj" "src\rs232.obj" "src\spi.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

