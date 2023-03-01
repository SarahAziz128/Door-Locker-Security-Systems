################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DoorLocker_HMI_ECU.c \
../gpio.c \
../internal_eeprom.c \
../keypad.c \
../lcd.c \
../timer.c \
../uart.c 

OBJS += \
./DoorLocker_HMI_ECU.o \
./gpio.o \
./internal_eeprom.o \
./keypad.o \
./lcd.o \
./timer.o \
./uart.o 

C_DEPS += \
./DoorLocker_HMI_ECU.d \
./gpio.d \
./internal_eeprom.d \
./keypad.d \
./lcd.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


