################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/src/bsp.c \
../bsp/src/factory_io.c 

OBJS += \
./bsp/src/bsp.o \
./bsp/src/factory_io.o 

C_DEPS += \
./bsp/src/bsp.d \
./bsp/src/factory_io.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/src/%.o bsp/src/%.su bsp/src/%.cyclo: ../bsp/src/%.c bsp/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F072xB -c -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/cmsis/device/inc" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/config" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/include" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/streamports/Jlink_RTT/config" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/FreeRTOS/include" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/bsp/inc" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/app/inc" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/cmsis/core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-bsp-2f-src

clean-bsp-2f-src:
	-$(RM) ./bsp/src/bsp.cyclo ./bsp/src/bsp.d ./bsp/src/bsp.o ./bsp/src/bsp.su ./bsp/src/factory_io.cyclo ./bsp/src/factory_io.d ./bsp/src/factory_io.o ./bsp/src/factory_io.su

.PHONY: clean-bsp-2f-src

