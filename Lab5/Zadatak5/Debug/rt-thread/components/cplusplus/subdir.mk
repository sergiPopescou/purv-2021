################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/cplusplus/crt_init.c 

CPP_SRCS += \
../rt-thread/components/cplusplus/Mutex.cpp \
../rt-thread/components/cplusplus/Semaphore.cpp \
../rt-thread/components/cplusplus/Thread.cpp \
../rt-thread/components/cplusplus/crt.cpp 

OBJS += \
./rt-thread/components/cplusplus/Mutex.o \
./rt-thread/components/cplusplus/Semaphore.o \
./rt-thread/components/cplusplus/Thread.o \
./rt-thread/components/cplusplus/crt.o \
./rt-thread/components/cplusplus/crt_init.o 

C_DEPS += \
./rt-thread/components/cplusplus/crt_init.d 

CPP_DEPS += \
./rt-thread/components/cplusplus/Mutex.d \
./rt-thread/components/cplusplus/Semaphore.d \
./rt-thread/components/cplusplus/Thread.d \
./rt-thread/components/cplusplus/crt.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/cplusplus/%.o: ../rt-thread/components/cplusplus/%.cpp
	arm-none-eabi-g++ -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -I"C:\RT-ThreadStudio\workspace\Zadatak5" -I"C:\RT-ThreadStudio\workspace\Zadatak5" -I"C:\RT-ThreadStudio\workspace\Zadatak5\applications" -I"C:\RT-ThreadStudio\workspace\Zadatak5\drivers" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\cplusplus" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\devfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\elmfat" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\ramfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\romfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi\sfud\inc" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\finsh" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\aio" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\compilers\newlib" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\mmap" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\pthreads" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\signal" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\termios" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\time" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\lwp" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\arch\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include\netif" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\netdev\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\impl" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\dfs_net" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket\sys_socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\common" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\cortex-a" -include"C:\RT-ThreadStudio\workspace\Zadatak5\rtconfig_preinc.h" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
rt-thread/components/cplusplus/%.o: ../rt-thread/components/cplusplus/%.c
	arm-none-eabi-gcc -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -DDEBUG -I"C:\RT-ThreadStudio\workspace\Zadatak5" -I"C:\RT-ThreadStudio\workspace\Zadatak5\applications" -I"C:\RT-ThreadStudio\workspace\Zadatak5\drivers" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\cplusplus" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\devfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\elmfat" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\ramfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\romfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi\sfud\inc" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\finsh" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\aio" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\compilers\newlib" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\mmap" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\pthreads" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\signal" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\termios" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\time" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\lwp" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\arch\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include\netif" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\netdev\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\impl" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\dfs_net" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket\sys_socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\common" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\cortex-a" -include"C:\RT-ThreadStudio\workspace\Zadatak5\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

