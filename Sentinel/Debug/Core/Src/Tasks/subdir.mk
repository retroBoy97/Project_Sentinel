################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Tasks/actuator_task.c \
../Core/Src/Tasks/comms_task.c \
../Core/Src/Tasks/display_task.c \
../Core/Src/Tasks/local_sensor_task.c \
../Core/Src/Tasks/safety_task.c \
../Core/Src/Tasks/sensor_poll_task.c \
../Core/Src/Tasks/watchdog_task.c 

OBJS += \
./Core/Src/Tasks/actuator_task.o \
./Core/Src/Tasks/comms_task.o \
./Core/Src/Tasks/display_task.o \
./Core/Src/Tasks/local_sensor_task.o \
./Core/Src/Tasks/safety_task.o \
./Core/Src/Tasks/sensor_poll_task.o \
./Core/Src/Tasks/watchdog_task.o 

C_DEPS += \
./Core/Src/Tasks/actuator_task.d \
./Core/Src/Tasks/comms_task.d \
./Core/Src/Tasks/display_task.d \
./Core/Src/Tasks/local_sensor_task.d \
./Core/Src/Tasks/safety_task.d \
./Core/Src/Tasks/sensor_poll_task.d \
./Core/Src/Tasks/watchdog_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Tasks/%.o Core/Src/Tasks/%.su Core/Src/Tasks/%.cyclo: ../Core/Src/Tasks/%.c Core/Src/Tasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Tasks

clean-Core-2f-Src-2f-Tasks:
	-$(RM) ./Core/Src/Tasks/actuator_task.cyclo ./Core/Src/Tasks/actuator_task.d ./Core/Src/Tasks/actuator_task.o ./Core/Src/Tasks/actuator_task.su ./Core/Src/Tasks/comms_task.cyclo ./Core/Src/Tasks/comms_task.d ./Core/Src/Tasks/comms_task.o ./Core/Src/Tasks/comms_task.su ./Core/Src/Tasks/display_task.cyclo ./Core/Src/Tasks/display_task.d ./Core/Src/Tasks/display_task.o ./Core/Src/Tasks/display_task.su ./Core/Src/Tasks/local_sensor_task.cyclo ./Core/Src/Tasks/local_sensor_task.d ./Core/Src/Tasks/local_sensor_task.o ./Core/Src/Tasks/local_sensor_task.su ./Core/Src/Tasks/safety_task.cyclo ./Core/Src/Tasks/safety_task.d ./Core/Src/Tasks/safety_task.o ./Core/Src/Tasks/safety_task.su ./Core/Src/Tasks/sensor_poll_task.cyclo ./Core/Src/Tasks/sensor_poll_task.d ./Core/Src/Tasks/sensor_poll_task.o ./Core/Src/Tasks/sensor_poll_task.su ./Core/Src/Tasks/watchdog_task.cyclo ./Core/Src/Tasks/watchdog_task.d ./Core/Src/Tasks/watchdog_task.o ./Core/Src/Tasks/watchdog_task.su

.PHONY: clean-Core-2f-Src-2f-Tasks

