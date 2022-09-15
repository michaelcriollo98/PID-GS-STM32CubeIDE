################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/A_star.c \
../Core/Src/cost_function.c \
../Core/Src/fgs_control.c \
../Core/Src/fuzzy.c \
../Core/Src/main.c \
../Core/Src/math_matriz.c \
../Core/Src/math_vector.c \
../Core/Src/performance_indices.c \
../Core/Src/pid.c \
../Core/Src/process_signal.c \
../Core/Src/random_numbers.c \
../Core/Src/simulated_annealing.c \
../Core/Src/state_space.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/A_star.o \
./Core/Src/cost_function.o \
./Core/Src/fgs_control.o \
./Core/Src/fuzzy.o \
./Core/Src/main.o \
./Core/Src/math_matriz.o \
./Core/Src/math_vector.o \
./Core/Src/performance_indices.o \
./Core/Src/pid.o \
./Core/Src/process_signal.o \
./Core/Src/random_numbers.o \
./Core/Src/simulated_annealing.o \
./Core/Src/state_space.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/A_star.d \
./Core/Src/cost_function.d \
./Core/Src/fgs_control.d \
./Core/Src/fuzzy.d \
./Core/Src/main.d \
./Core/Src/math_matriz.d \
./Core/Src/math_vector.d \
./Core/Src/performance_indices.d \
./Core/Src/pid.d \
./Core/Src/process_signal.d \
./Core/Src/random_numbers.d \
./Core/Src/simulated_annealing.d \
./Core/Src/state_space.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/A_star.d ./Core/Src/A_star.o ./Core/Src/A_star.su ./Core/Src/cost_function.d ./Core/Src/cost_function.o ./Core/Src/cost_function.su ./Core/Src/fgs_control.d ./Core/Src/fgs_control.o ./Core/Src/fgs_control.su ./Core/Src/fuzzy.d ./Core/Src/fuzzy.o ./Core/Src/fuzzy.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/math_matriz.d ./Core/Src/math_matriz.o ./Core/Src/math_matriz.su ./Core/Src/math_vector.d ./Core/Src/math_vector.o ./Core/Src/math_vector.su ./Core/Src/performance_indices.d ./Core/Src/performance_indices.o ./Core/Src/performance_indices.su ./Core/Src/pid.d ./Core/Src/pid.o ./Core/Src/pid.su ./Core/Src/process_signal.d ./Core/Src/process_signal.o ./Core/Src/process_signal.su ./Core/Src/random_numbers.d ./Core/Src/random_numbers.o ./Core/Src/random_numbers.su ./Core/Src/simulated_annealing.d ./Core/Src/simulated_annealing.o ./Core/Src/simulated_annealing.su ./Core/Src/state_space.d ./Core/Src/state_space.o ./Core/Src/state_space.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

