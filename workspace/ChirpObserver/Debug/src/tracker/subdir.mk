################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tracker/Chirp.cpp \
../src/tracker/GeneralTracker.cpp \
../src/tracker/Tracker.cpp 

CPP_DEPS += \
./src/tracker/Chirp.d \
./src/tracker/GeneralTracker.d \
./src/tracker/Tracker.d 


# Each subdirectory must supply rules for building sources it contributes
src/tracker/%.o: ../src/tracker/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/robotlabben/Downloads/OpenCV/opencv-2.4.9/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


