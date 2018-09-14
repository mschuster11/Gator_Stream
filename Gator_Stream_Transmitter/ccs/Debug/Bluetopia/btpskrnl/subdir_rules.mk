################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Bluetopia/btpskrnl/BTPSKRNL.obj: C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/btpskrnl/BTPSKRNL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=0 --include_path="C:/ti/ccsv8/ccs_base/arm/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Platforms/driverlib" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/ccs" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Platforms/MSP-EXP432P401R" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/hcitrans" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/btvs" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/btpskrnl" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/btpsvend" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/profiles/a2dp/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/profiles/audio/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/profiles/avctp/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/profiles/avrcp/include" --include_path="C:/Users/ghost/OneDrive/School-Master/EEL4924C/Gator_Stream_Transmitter/Bluetopia/profiles/gavd/include" --include_path="C:/ti/ccsv8/ccs_base/arm/include/CMSIS" --preinclude="msp432p401r.h" --advice:power=all --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --define=__SUPPORT_AUDIO_CODEC__ --define=__SUPPORT_AVPR__ -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Bluetopia/btpskrnl/BTPSKRNL.d_raw" --obj_directory="Bluetopia/btpskrnl" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


