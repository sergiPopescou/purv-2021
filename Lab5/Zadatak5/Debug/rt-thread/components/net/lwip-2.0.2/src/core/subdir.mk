################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/net/lwip-2.0.2/src/core/def.c \
../rt-thread/components/net/lwip-2.0.2/src/core/dns.c \
../rt-thread/components/net/lwip-2.0.2/src/core/inet_chksum.c \
../rt-thread/components/net/lwip-2.0.2/src/core/init.c \
../rt-thread/components/net/lwip-2.0.2/src/core/ip.c \
../rt-thread/components/net/lwip-2.0.2/src/core/memp.c \
../rt-thread/components/net/lwip-2.0.2/src/core/netif.c \
../rt-thread/components/net/lwip-2.0.2/src/core/pbuf.c \
../rt-thread/components/net/lwip-2.0.2/src/core/raw.c \
../rt-thread/components/net/lwip-2.0.2/src/core/stats.c \
../rt-thread/components/net/lwip-2.0.2/src/core/sys.c \
../rt-thread/components/net/lwip-2.0.2/src/core/tcp.c \
../rt-thread/components/net/lwip-2.0.2/src/core/tcp_in.c \
../rt-thread/components/net/lwip-2.0.2/src/core/tcp_out.c \
../rt-thread/components/net/lwip-2.0.2/src/core/timeouts.c \
../rt-thread/components/net/lwip-2.0.2/src/core/udp.c 

OBJS += \
./rt-thread/components/net/lwip-2.0.2/src/core/def.o \
./rt-thread/components/net/lwip-2.0.2/src/core/dns.o \
./rt-thread/components/net/lwip-2.0.2/src/core/inet_chksum.o \
./rt-thread/components/net/lwip-2.0.2/src/core/init.o \
./rt-thread/components/net/lwip-2.0.2/src/core/ip.o \
./rt-thread/components/net/lwip-2.0.2/src/core/memp.o \
./rt-thread/components/net/lwip-2.0.2/src/core/netif.o \
./rt-thread/components/net/lwip-2.0.2/src/core/pbuf.o \
./rt-thread/components/net/lwip-2.0.2/src/core/raw.o \
./rt-thread/components/net/lwip-2.0.2/src/core/stats.o \
./rt-thread/components/net/lwip-2.0.2/src/core/sys.o \
./rt-thread/components/net/lwip-2.0.2/src/core/tcp.o \
./rt-thread/components/net/lwip-2.0.2/src/core/tcp_in.o \
./rt-thread/components/net/lwip-2.0.2/src/core/tcp_out.o \
./rt-thread/components/net/lwip-2.0.2/src/core/timeouts.o \
./rt-thread/components/net/lwip-2.0.2/src/core/udp.o 

C_DEPS += \
./rt-thread/components/net/lwip-2.0.2/src/core/def.d \
./rt-thread/components/net/lwip-2.0.2/src/core/dns.d \
./rt-thread/components/net/lwip-2.0.2/src/core/inet_chksum.d \
./rt-thread/components/net/lwip-2.0.2/src/core/init.d \
./rt-thread/components/net/lwip-2.0.2/src/core/ip.d \
./rt-thread/components/net/lwip-2.0.2/src/core/memp.d \
./rt-thread/components/net/lwip-2.0.2/src/core/netif.d \
./rt-thread/components/net/lwip-2.0.2/src/core/pbuf.d \
./rt-thread/components/net/lwip-2.0.2/src/core/raw.d \
./rt-thread/components/net/lwip-2.0.2/src/core/stats.d \
./rt-thread/components/net/lwip-2.0.2/src/core/sys.d \
./rt-thread/components/net/lwip-2.0.2/src/core/tcp.d \
./rt-thread/components/net/lwip-2.0.2/src/core/tcp_in.d \
./rt-thread/components/net/lwip-2.0.2/src/core/tcp_out.d \
./rt-thread/components/net/lwip-2.0.2/src/core/timeouts.d \
./rt-thread/components/net/lwip-2.0.2/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/net/lwip-2.0.2/src/core/%.o: ../rt-thread/components/net/lwip-2.0.2/src/core/%.c
	arm-none-eabi-gcc -march=armv7-a -marm -mfloat-abi=soft -O0 -Wall  -g -gdwarf-2 -DDEBUG -I"C:\RT-ThreadStudio\workspace\Zadatak5" -I"C:\RT-ThreadStudio\workspace\Zadatak5\applications" -I"C:\RT-ThreadStudio\workspace\Zadatak5\drivers" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\cplusplus" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\devfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\elmfat" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\ramfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\filesystems\romfs" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\dfs\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi\sfud\inc" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\drivers\spi" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\finsh" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\aio" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\compilers\newlib" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\mmap" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\pthreads" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\signal" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\termios" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\libc\time" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\lwp" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\arch\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include\netif" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\lwip-2.0.2\src" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\netdev\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\impl" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\dfs_net" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket\sys_socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include\socket" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\components\net\sal_socket\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\include" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\common" -I"C:\RT-ThreadStudio\workspace\Zadatak5\rt-thread\libcpu\arm\cortex-a" -include"C:\RT-ThreadStudio\workspace\Zadatak5\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

