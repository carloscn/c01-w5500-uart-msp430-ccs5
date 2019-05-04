################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/W5500.obj: ../src/W5500.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi --include_path="C:/opt/ti/ccsv5/ccs_base/msp430/include" --include_path="../inc" --include_path="C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F149__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/W5500.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.obj: ../src/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi --include_path="C:/opt/ti/ccsv5/ccs_base/msp430/include" --include_path="../inc" --include_path="C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F149__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/main.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rs232.obj: ../src/rs232.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi --include_path="C:/opt/ti/ccsv5/ccs_base/msp430/include" --include_path="../inc" --include_path="C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F149__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/rs232.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/spi.obj: ../src/spi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi --include_path="C:/opt/ti/ccsv5/ccs_base/msp430/include" --include_path="../inc" --include_path="C:/opt/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F149__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/spi.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


