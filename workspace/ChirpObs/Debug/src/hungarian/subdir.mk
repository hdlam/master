################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/hungarian/Hungarian.cpp 

OBJS += \
./src/hungarian/Hungarian.o 

CPP_DEPS += \
./src/hungarian/Hungarian.d 


# Each subdirectory must supply rules for building sources it contributes
src/hungarian/%.o: ../src/hungarian/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


