################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/MemMang/heap_1.c 

OBJS += \
./FreeRTOS/portable/MemMang/heap_1.o 

C_DEPS += \
./FreeRTOS/portable/MemMang/heap_1.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/MemMang/%.o FreeRTOS/portable/MemMang/%.su FreeRTOS/portable/MemMang/%.cyclo: ../FreeRTOS/portable/MemMang/%.c FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F072xB -c -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/cmsis/device/inc" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/config" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/include" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/streamports/Jlink_RTT/config" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/FreeRTOS/include" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/bsp/inc" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/app/inc" -I"C:/Users/pierr/Desktop/STM/STM_tuto_import/paletiseur/cmsis/core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-FreeRTOS-2f-portable-2f-MemMang

clean-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./FreeRTOS/portable/MemMang/heap_1.cyclo ./FreeRTOS/portable/MemMang/heap_1.d ./FreeRTOS/portable/MemMang/heap_1.o ./FreeRTOS/portable/MemMang/heap_1.su

.PHONY: clean-FreeRTOS-2f-portable-2f-MemMang

