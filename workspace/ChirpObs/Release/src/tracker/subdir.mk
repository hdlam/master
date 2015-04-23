################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tracker/Chirp.cpp \
../src/tracker/GeneralTracker.cpp \
../src/tracker/Tracker.cpp 

OBJS += \
./src/tracker/Chirp.o \
./src/tracker/GeneralTracker.o \
./src/tracker/Tracker.o 

CPP_DEPS += \
./src/tracker/Chirp.d \
./src/tracker/GeneralTracker.d \
./src/tracker/Tracker.d 


# Each subdirectory must supply rules for building sources it contributes
src/tracker/%.o: ../src/tracker/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


