################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/rs232/rs232.c 

OBJS += \
./src/rs232/rs232.o 

C_DEPS += \
./src/rs232/rs232.d 


# Each subdirectory must supply rules for building sources it contributes
src/rs232/%.o: ../src/rs232/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


