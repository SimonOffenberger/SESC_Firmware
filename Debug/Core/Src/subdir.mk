################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Park_u_Clark.c \
../Core/Src/SVM.c \
../Core/Src/adc.c \
../Core/Src/dma.c \
../Core/Src/eeprom.c \
../Core/Src/encoder.c \
../Core/Src/global_var.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/phaseEstimater.c \
../Core/Src/stm32f3xx_hal_msp.c \
../Core/Src/stm32f3xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f3xx.c \
../Core/Src/tim.c \
../Core/Src/uart_debug.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/Park_u_Clark.o \
./Core/Src/SVM.o \
./Core/Src/adc.o \
./Core/Src/dma.o \
./Core/Src/eeprom.o \
./Core/Src/encoder.o \
./Core/Src/global_var.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/phaseEstimater.o \
./Core/Src/stm32f3xx_hal_msp.o \
./Core/Src/stm32f3xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f3xx.o \
./Core/Src/tim.o \
./Core/Src/uart_debug.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/Park_u_Clark.d \
./Core/Src/SVM.d \
./Core/Src/adc.d \
./Core/Src/dma.d \
./Core/Src/eeprom.d \
./Core/Src/encoder.d \
./Core/Src/global_var.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/phaseEstimater.d \
./Core/Src/stm32f3xx_hal_msp.d \
./Core/Src/stm32f3xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f3xx.d \
./Core/Src/tim.d \
./Core/Src/uart_debug.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include -I"D:/Ablage-Sync/Zuhause/CubeIDEWorkspace/SESC_V2_Firmware/Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Park_u_Clark.d ./Core/Src/Park_u_Clark.o ./Core/Src/SVM.d ./Core/Src/SVM.o ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/encoder.d ./Core/Src/encoder.o ./Core/Src/global_var.d ./Core/Src/global_var.o ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/phaseEstimater.d ./Core/Src/phaseEstimater.o ./Core/Src/stm32f3xx_hal_msp.d ./Core/Src/stm32f3xx_hal_msp.o ./Core/Src/stm32f3xx_it.d ./Core/Src/stm32f3xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f3xx.d ./Core/Src/system_stm32f3xx.o ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/uart_debug.d ./Core/Src/uart_debug.o ./Core/Src/usart.d ./Core/Src/usart.o

.PHONY: clean-Core-2f-Src

