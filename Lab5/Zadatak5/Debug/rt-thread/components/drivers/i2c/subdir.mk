################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/drivers/i2c/i2c-bit-ops.c \
../rt-thread/components/drivers/i2c/i2c_core.c \
../rt-thread/components/drivers/i2c/i2c_dev.c 

OBJS += \
./rt-thread/components/drivers/i2c/i2c-bit-ops.o \
./rt-thread/components/drivers/i2c/i2c_core.o \
./rt-thread/components/drivers/i2c/i2c_dev.o 

C_DEPS += \
./rt-thread/components/drivers/i2c/i2c-bit-ops.d \
./rt-thread/components/drivers/i2c/i2c_core.d \
./rt-thread/components/drivers/i2c/i2c_dev.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/drivers/i2c/%.o: ../rt-thread/components/drivers/i2c/%.c
	arm-none-eabi-gcc -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -DDEBUG -I"C:\RT-ThreadStudio\workspace\Zadatak5" -I"C:\RT-ThreadStudio\workspace\Zadatak5\applications" -I"C:\RT-ThreadStudio\workspace\Zadatak5\drivers" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\cplusplus" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\devfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\elmfat" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\ramfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\romfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi\sfud\inc" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\finsh" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\aio" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\compilers\newlib" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\mmap" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\pthreads" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\signal" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\termios" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\time" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\lwp" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\arch\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include\netif" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\netdev\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\impl" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\dfs_net" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket\sys_socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\common" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\cortex-a" -include"C:\RT-ThreadStudio\workspace\Zadatak5\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

