#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define THREAD_STACK_SIZE 5000
#define THREAD_TIME_SLICE 10
#define THREAD_PRIO 5

#define ACCL_GYRO_MAX 255

#define THERMO_LOW_LIMIT 15
#define THERMO_UP_LIMIT 25
#define ACCL_LIMIT 200
#define GYRO_LIMIT 190
#define RISK_LIMIT 16e3

#define TEMP_LOWER_CHANGE -3
#define TEMP_UPPER_CHANGE 3

#define PRODUCTION_LIMIT 100

#define ACCL_SLEEP 20
#define GYRO_SLEEP 20
#define THERMO_SLEEP 20
#define OUT_SLEEP 50

static rt_uint32_t accl_buffer[PRODUCTION_LIMIT];
static rt_uint32_t gyro_buffer[PRODUCTION_LIMIT];
static rt_uint32_t thermo_buffer[PRODUCTION_LIMIT];
static char out_buffer[4*PRODUCTION_LIMIT][20];

static rt_uint32_t index_accl=0;
static rt_uint32_t index_gyro=0;
static rt_uint32_t index_thermo=0;
static rt_uint32_t index_out=0;
static rt_uint32_t index_watch=0;

static rt_uint32_t index_accl_take=0;
static rt_uint32_t index_gyro_take=0;
static rt_uint32_t index_thermo_take=0;

static rt_int32_t temperature=20;

static rt_uint32_t write_flag=0;
static rt_uint32_t write_to_the_end=0;

static rt_mutex_t accl_mux=RT_NULL;
static rt_mutex_t gyro_mux=RT_NULL;
static rt_mutex_t thermo_mux=RT_NULL;
static rt_mutex_t out_mux=RT_NULL;

static rt_thread_t accelerometer=RT_NULL;
static rt_thread_t gyroscope=RT_NULL;
static rt_thread_t watchdog=RT_NULL;
static rt_thread_t thermometer=RT_NULL;
static rt_thread_t output=RT_NULL;

static void accl_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(accl_mux, RT_WAITING_FOREVER);
            accl_buffer[index_accl++]=1+rand()%ACCL_GYRO_MAX;
        rt_mutex_release(accl_mux);

        if(index_accl==PRODUCTION_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(ACCL_SLEEP);
    }
}

static void gyro_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(gyro_mux, RT_WAITING_FOREVER);
            gyro_buffer[index_gyro++]=1+rand()%ACCL_GYRO_MAX;
        rt_mutex_release(gyro_mux);

        if(index_gyro==PRODUCTION_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(GYRO_SLEEP);
    }
}

static void thermo_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(thermo_mux, RT_WAITING_FOREVER);
            thermo_buffer[index_thermo++]=temperature+TEMP_LOWER_CHANGE+(rand()%(TEMP_UPPER_CHANGE-TEMP_LOWER_CHANGE+1));
        rt_mutex_release(thermo_mux);

        if(index_thermo==PRODUCTION_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(THERMO_SLEEP);
    }
}

static void watch_enter(void* param)
{
    rt_uint32_t accl_carry_buffer[PRODUCTION_LIMIT];
    rt_uint32_t gyro_carry_buffer[PRODUCTION_LIMIT];

    rt_uint32_t index_carry_accl=0;
    rt_uint32_t index_carry_gyro=0;

    rt_uint32_t accl_flag=0;
    rt_uint32_t gyro_flag=0;
    rt_uint32_t thermo_flag=0;

    while(1)
    {
        rt_uint32_t pom_accl;
        rt_uint32_t pom_gyro;
        rt_uint32_t pom_thermo;

        rt_mutex_take(thermo_mux, RT_WAITING_FOREVER);
            if(index_thermo_take<index_thermo)
            {
                pom_thermo=thermo_buffer[index_thermo_take++];
                thermo_flag=1;
            }
        rt_mutex_release(thermo_mux);

        rt_mutex_take(gyro_mux, RT_WAITING_FOREVER);
            if(index_gyro_take<index_gyro)
            {
                pom_gyro=gyro_buffer[index_gyro_take++];
                gyro_carry_buffer[index_carry_gyro++]=pom_gyro;
                gyro_flag=1;
            }
        rt_mutex_release(gyro_mux);

        rt_mutex_take(accl_mux, RT_WAITING_FOREVER);
            if(index_accl_take<index_thermo)
            {
                pom_accl=accl_buffer[index_accl_take++];
                accl_carry_buffer[index_carry_accl++]=pom_accl;
                accl_flag=1;
            }
        rt_mutex_release(accl_mux);

        rt_mutex_take(out_mux, RT_WAITING_FOREVER);
            if(gyro_flag==1 || thermo_flag==1 || accl_flag==1)
            {
                write_flag=1;
            }
            if(accl_flag==1 && gyro_flag==1)
            {
                sprintf(out_buffer[index_watch++],"RISK:%d:%s",pom_gyro*pom_accl,pom_gyro*pom_accl<RISK_LIMIT ? "OK" : "NOK");
            }
            if(accl_flag==1)
            {
                accl_flag=0;
                sprintf(out_buffer[index_watch++],"ACCL:%d:%s",pom_accl,pom_accl<ACCL_LIMIT ? "OK" : "NOK");
                if(index_carry_gyro>1)
                {
                    for(int i=0;i<index_carry_gyro;++i)
                    {
                        sprintf(out_buffer[index_watch++],"RISK:%d:%s",gyro_carry_buffer[i]*pom_accl,gyro_carry_buffer[i]*pom_accl<RISK_LIMIT ? "OK" : "NOK");
                    }
                    index_carry_gyro=0;
                }
                else
                {
                    index_carry_gyro=0;
                }
            }

            if(gyro_flag==1)
            {
                gyro_flag=0;
                sprintf(out_buffer[index_watch++],"GYRO:%d:%s",pom_gyro,pom_gyro<GYRO_LIMIT ? "OK" : "NOK");
                if(index_carry_accl>1)
                {
                    for(int i=0;i<index_carry_accl;++i)
                    {
                        sprintf(out_buffer[index_watch++],"RISK:%d:%s",accl_carry_buffer[i]*pom_gyro,accl_carry_buffer[i]*pom_gyro<RISK_LIMIT ? "OK" : "NOK");
                    }
                    index_carry_accl=0;
                }
                else
                {
                    index_carry_accl=0;
                }
            }

            if(thermo_flag==1)
            {
                sprintf(out_buffer[index_watch++],"THERMO:%d:%s",pom_thermo,pom_thermo>=THERMO_LOW_LIMIT && pom_thermo<=THERMO_UP_LIMIT ? "OK" : "NOK");
            }
        rt_mutex_release(out_mux);

        if(index_watch==4*PRODUCTION_LIMIT)
        {
            write_to_the_end=1;
            break;
        }
        rt_thread_mdelay(1+rand()%5);
    }
}

static void out_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(out_mux, RT_WAITING_FOREVER);
            if(write_flag==1 || write_to_the_end==1)
            {
                rt_kprintf("%s\n",out_buffer[index_out++]);
                write_flag=0;
            }
        rt_mutex_release(out_mux);
        if(index_out==4*PRODUCTION_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(OUT_SLEEP);
    }
    rt_kprintf("KRAJ");
}

int main(int argc, char **argv)
{
    accelerometer=rt_thread_create("A", accl_enter, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIO, THREAD_STACK_SIZE);
    gyroscope=rt_thread_create("B", gyro_enter, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIO, THREAD_STACK_SIZE);
    thermometer=rt_thread_create("C", thermo_enter, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIO, THREAD_STACK_SIZE);
    output=rt_thread_create("D", out_enter, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIO, THREAD_STACK_SIZE);
    watchdog=rt_thread_create("E", watch_enter, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIO, THREAD_STACK_SIZE);

    accl_mux=rt_mutex_create("a", RT_IPC_FLAG_FIFO);
    gyro_mux=rt_mutex_create("b", RT_IPC_FLAG_FIFO);
    thermo_mux=rt_mutex_create("c", RT_IPC_FLAG_FIFO);
    out_mux=rt_mutex_create("d", RT_IPC_FLAG_FIFO);

    if(accl_mux==RT_NULL)
    {
        rt_kprintf("Faild to create mutex!");
        return -1;
    }

    if(gyro_mux==RT_NULL)
    {
        rt_kprintf("Faild to create mutex!");
        return -1;
    }

    if(thermo_mux==RT_NULL)
    {
        rt_kprintf("Faild to create mutex!");
        return -1;
    }

    if(out_mux==RT_NULL)
    {
        rt_kprintf("Faild to create mutex!");
        return -1;
    }

    if(accelerometer!=RT_NULL)
    {
        rt_thread_startup(accelerometer);
    }
    else
    {
        rt_kprintf("Accelerometer faild to start!");
        return -1;
    }

    if(gyroscope!=RT_NULL)
    {
        rt_thread_startup(gyroscope);
    }
    else
    {
        rt_kprintf("Gyroscope faild to start!");
        return -1;
    }

    if(thermometer!=RT_NULL)
    {
        rt_thread_startup(thermometer);
    }
    else
    {
        rt_kprintf("Thermometer faild to start!");
        return -1;
    }
    if(output!=RT_NULL)
    {
        rt_thread_startup(output);
    }
    else
    {
        rt_kprintf("Output faild to start!");
        return -1;
    }
    if(watchdog!=RT_NULL)
    {
        rt_thread_startup(watchdog);
    }
    else
    {
        rt_kprintf("Watchdog faild to start!");
        return -1;
    }
    return 0;
}
