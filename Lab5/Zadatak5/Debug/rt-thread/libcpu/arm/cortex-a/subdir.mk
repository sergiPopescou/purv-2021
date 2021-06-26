################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/libcpu/arm/cortex-a/cache.c \
../rt-thread/libcpu/arm/cortex-a/cpu.c \
../rt-thread/libcpu/arm/cortex-a/gic.c \
../rt-thread/libcpu/arm/cortex-a/interrupt.c \
../rt-thread/libcpu/arm/cortex-a/mmu.c \
../rt-thread/libcpu/arm/cortex-a/pmu.c \
../rt-thread/libcpu/arm/cortex-a/stack.c \
../rt-thread/libcpu/arm/cortex-a/trap.c 

S_UPPER_SRCS += \
../rt-thread/libcpu/arm/cortex-a/context_gcc.S \
../rt-thread/libcpu/arm/cortex-a/cp15_gcc.S \
../rt-thread/libcpu/arm/cortex-a/start_gcc.S \
../rt-thread/libcpu/arm/cortex-a/vector_gcc.S 

OBJS += \
./rt-thread/libcpu/arm/cortex-a/cache.o \
./rt-thread/libcpu/arm/cortex-a/context_gcc.o \
./rt-thread/libcpu/arm/cortex-a/cp15_gcc.o \
./rt-thread/libcpu/arm/cortex-a/cpu.o \
./rt-thread/libcpu/arm/cortex-a/gic.o \
./rt-thread/libcpu/arm/cortex-a/interrupt.o \
./rt-thread/libcpu/arm/cortex-a/mmu.o \
./rt-thread/libcpu/arm/cortex-a/pmu.o \
./rt-thread/libcpu/arm/cortex-a/stack.o \
./rt-thread/libcpu/arm/cortex-a/start_gcc.o \
./rt-thread/libcpu/arm/cortex-a/trap.o \
./rt-thread/libcpu/arm/cortex-a/vector_gcc.o 

S_UPPER_DEPS += \
./rt-thread/libcpu/arm/cortex-a/context_gcc.d \
./rt-thread/libcpu/arm/cortex-a/cp15_gcc.d \
./rt-thread/libcpu/arm/cortex-a/start_gcc.d \
./rt-thread/libcpu/arm/cortex-a/vector_gcc.d 

C_DEPS += \
./rt-thread/libcpu/arm/cortex-a/cache.d \
./rt-thread/libcpu/arm/cortex-a/cpu.d \
./rt-thread/libcpu/arm/cortex-a/gic.d \
./rt-thread/libcpu/arm/cortex-a/interrupt.d \
./rt-thread/libcpu/arm/cortex-a/mmu.d \
./rt-thread/libcpu/arm/cortex-a/pmu.d \
./rt-thread/libcpu/arm/cortex-a/stack.d \
./rt-thread/libcpu/arm/cortex-a/trap.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/libcpu/arm/cortex-a/%.o: ../rt-thread/libcpu/arm/cortex-a/%.c
	arm-none-eabi-gcc -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -DDEBUG -I"C:\RT-ThreadStudio\workspace\Zadatak5" -I"C:\RT-ThreadStudio\workspace\Zadatak5\applications" -I"C:\RT-ThreadStudio\workspace\Zadatak5\drivers" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\cplusplus" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\devfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\elmfat" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\ramfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\romfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi\sfud\inc" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\finsh" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\aio" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\compilers\newlib" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\mmap" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\pthreads" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\signal" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\termios" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\time" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\lwp" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\arch\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include\netif" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\netdev\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\impl" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\dfs_net" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket\sys_socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\common" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\cortex-a" -include"C:\RT-ThreadStudio\workspace\Zadatak5\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
rt-thread/libcpu/arm/cortex-a/%.o: ../rt-thread/libcpu/arm/cortex-a/%.S
	arm-none-eabi-gcc -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -x assembler-with-cpp -D__ASSEMBLY__ -I"C:\RT-ThreadStudio\workspace\Zadatak5" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

