################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../rt-thread/components/lwp/arch/arm/cortex-a/lwp_gcc.S 

OBJS += \
./rt-thread/components/lwp/arch/arm/cortex-a/lwp_gcc.o 

S_UPPER_DEPS += \
./rt-thread/components/lwp/arch/arm/cortex-a/lwp_gcc.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/lwp/arch/arm/cortex-a/%.o: ../rt-thread/components/lwp/arch/arm/cortex-a/%.S
	arm-none-eabi-gcc -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -x assembler-with-cpp -D__ASSEMBLY__ -I"C:\RT-ThreadStudio\workspace\Zadatak5" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

