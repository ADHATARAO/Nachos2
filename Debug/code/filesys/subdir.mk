################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../code/filesys/directory.cc \
../code/filesys/filehdr.cc \
../code/filesys/filesys.cc \
../code/filesys/fstest.cc \
../code/filesys/openfile.cc \
../code/filesys/synchdisk.cc 

OBJS += \
./code/filesys/directory.o \
./code/filesys/filehdr.o \
./code/filesys/filesys.o \
./code/filesys/fstest.o \
./code/filesys/openfile.o \
./code/filesys/synchdisk.o 

CC_DEPS += \
./code/filesys/directory.d \
./code/filesys/filehdr.d \
./code/filesys/filesys.d \
./code/filesys/fstest.d \
./code/filesys/openfile.d \
./code/filesys/synchdisk.d 


# Each subdirectory must supply rules for building sources it contributes
code/filesys/%.o: ../code/filesys/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


