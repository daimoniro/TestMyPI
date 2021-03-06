################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/compass.c \
../source/debug.c \
../source/distance.c \
../source/gestioneIO.c \
../source/gestioneMotoriDC.c \
../source/gestioneMotoriStepper.c \
../source/gestioneServo.c \
../source/gyroAccelerometer.c \
../source/log.c \
../source/main.c \
../source/serverTCP.c \
../source/tempHumSensor.c \
../source/udp.c \
../source/udpServer.c 

OBJS += \
./source/compass.o \
./source/debug.o \
./source/distance.o \
./source/gestioneIO.o \
./source/gestioneMotoriDC.o \
./source/gestioneMotoriStepper.o \
./source/gestioneServo.o \
./source/gyroAccelerometer.o \
./source/log.o \
./source/main.o \
./source/serverTCP.o \
./source/tempHumSensor.o \
./source/udp.o \
./source/udpServer.o 

C_DEPS += \
./source/compass.d \
./source/debug.d \
./source/distance.d \
./source/gestioneIO.d \
./source/gestioneMotoriDC.d \
./source/gestioneMotoriStepper.d \
./source/gestioneServo.d \
./source/gyroAccelerometer.d \
./source/log.d \
./source/main.d \
./source/serverTCP.d \
./source/tempHumSensor.d \
./source/udp.d \
./source/udpServer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/pi/workspace/TestMyPI/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


