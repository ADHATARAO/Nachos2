################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../code/userprog/addrspace.cc \
../code/userprog/bitmap.cc \
../code/userprog/exception.cc \
../code/userprog/fork.cc \
../code/userprog/progtest.cc \
../code/userprog/synchconsole.cc \
../code/userprog/userthread.cc 

OBJS += \
./code/userprog/addrspace.o \
./code/userprog/bitmap.o \
./code/userprog/exception.o \
./code/userprog/fork.o \
./code/userprog/progtest.o \
./code/userprog/synchconsole.o \
./code/userprog/userthread.o 

CC_DEPS += \
./code/userprog/addrspace.d \
./code/userprog/bitmap.d \
./code/userprog/exception.d \
./code/userprog/fork.d \
./code/userprog/progtest.d \
./code/userprog/synchconsole.d \
./code/userprog/userthread.d 


# Each subdirectory must supply rules for building sources it contributes
code/userprog/%.o: ../code/userprog/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


