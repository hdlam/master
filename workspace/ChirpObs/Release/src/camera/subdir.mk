################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/camera/BoundingBoxHandler.cpp \
../src/camera/CameraHandler.cpp \
../src/camera/Led.cpp 

OBJS += \
./src/camera/BoundingBoxHandler.o \
./src/camera/CameraHandler.o \
./src/camera/Led.o 

CPP_DEPS += \
./src/camera/BoundingBoxHandler.d \
./src/camera/CameraHandler.d \
./src/camera/Led.d 


# Each subdirectory must supply rules for building sources it contributes
src/camera/%.o: ../src/camera/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


