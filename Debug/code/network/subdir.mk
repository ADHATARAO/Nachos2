################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../code/network/nettest.cc \
../code/network/post.cc 

OBJS += \
./code/network/nettest.o \
./code/network/post.o 

CC_DEPS += \
./code/network/nettest.d \
./code/network/post.d 


# Each subdirectory must supply rules for building sources it contributes
code/network/%.o: ../code/network/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


