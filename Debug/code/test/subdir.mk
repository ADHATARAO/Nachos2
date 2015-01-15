################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/test/getint.c \
../code/test/getstring.c \
../code/test/halt.c \
../code/test/makethreads.c \
../code/test/matmult.c \
../code/test/putchar.c \
../code/test/putint.c \
../code/test/putstring.c \
../code/test/shell.c \
../code/test/sort.c 

S_UPPER_SRCS += \
../code/test/start.S \
../code/test/start.S.BACKUP.6292.S \
../code/test/start.S.BASE.6292.S \
../code/test/start.S.LOCAL.6292.S \
../code/test/start.S.REMOTE.6292.S 

OBJS += \
./code/test/getint.o \
./code/test/getstring.o \
./code/test/halt.o \
./code/test/makethreads.o \
./code/test/matmult.o \
./code/test/putchar.o \
./code/test/putint.o \
./code/test/putstring.o \
./code/test/shell.o \
./code/test/sort.o \
./code/test/start.o \
./code/test/start.S.BACKUP.6292.o \
./code/test/start.S.BASE.6292.o \
./code/test/start.S.LOCAL.6292.o \
./code/test/start.S.REMOTE.6292.o 

C_DEPS += \
./code/test/getint.d \
./code/test/getstring.d \
./code/test/halt.d \
./code/test/makethreads.d \
./code/test/matmult.d \
./code/test/putchar.d \
./code/test/putint.d \
./code/test/putstring.d \
./code/test/shell.d \
./code/test/sort.d 


# Each subdirectory must supply rules for building sources it contributes
code/test/%.o: ../code/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

code/test/%.o: ../code/test/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


