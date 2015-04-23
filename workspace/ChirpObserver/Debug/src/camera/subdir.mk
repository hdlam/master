################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/camera/BoundingBoxHandler.cpp \
../src/camera/CameraHandler.cpp \
../src/camera/Led.cpp 

CPP_DEPS += \
./src/camera/BoundingBoxHandler.d \
./src/camera/CameraHandler.d \
./src/camera/Led.d 


# Each subdirectory must supply rules for building sources it contributes
src/camera/%.o: ../src/camera/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/robotlabben/Downloads/OpenCV/opencv-2.4.9/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


