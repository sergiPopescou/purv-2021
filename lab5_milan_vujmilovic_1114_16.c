#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define PROD_LIMIT 100

static rt_uint32_t accl_buffer[PROD_LIMIT];
static rt_uint32_t gyro_buffer[PROD_LIMIT];
static rt_uint32_t thermo_buffer[PROD_LIMIT];
static char out_buffer[4*PROD_LIMIT][20];

static rt_uint32_t accl_index=0;
static rt_uint32_t gyro_index=0;
static rt_uint32_t thermo_index=0;
static rt_uint32_t out_index=0;
static rt_uint32_t watch_index=0;

static rt_uint32_t accl_index_take=0;
static rt_uint32_t gyro_index_take=0;
static rt_uint32_t thermo_index_take=0;

static rt_int32_t temp=20;

static rt_uint32_t write_flag=0;
static rt_uint32_t write_to_the_end=0;

static rt_mutex_t accl=RT_NULL;
static rt_mutex_t gyro=RT_NULL;
static rt_mutex_t thermo=RT_NULL;
static rt_mutex_t out=RT_NULL;

static rt_thread_t accelerometer=RT_NULL;
static rt_thread_t gyroscope=RT_NULL;
static rt_thread_t watchdog=RT_NULL;
static rt_thread_t thermometer=RT_NULL;
static rt_thread_t output=RT_NULL;

#define temp_lower_change -3
#define temp_upper_change 3

#define accl_sleep 20
#define gyro_sleep 20
#define thermo_sleep 20
#define out_sleep 50

#define thread_stack_size 10000
#define thread_slice_size 10
#define thread_prio 5

#define accl_gyro_max 255

#define thermo_low_limit 15
#define thermo_up_limit 25
#define accl_limit 200
#define gyro_limit 190
#define risk_limit 16e3

static void accl_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(accl, RT_WAITING_FOREVER);
            accl_buffer[accl_index++]=1+rand()%accl_gyro_max;
        rt_mutex_release(accl);

        if(accl_index==PROD_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(accl_sleep);
    }
}

static void gyro_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(gyro, RT_WAITING_FOREVER);
            gyro_buffer[gyro_index++]=1+rand()%accl_gyro_max;
        rt_mutex_release(gyro);

        if(gyro_index==PROD_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(gyro_sleep);
    }
}

static void thermo_enter(void* param)
{
    while(1)
    {
        rt_mutex_take(thermo, RT_WAITING_FOREVER);
            thermo_buffer[thermo_index++]=temp+temp_lower_change+(rand()%(temp_upper_change-temp_lower_change+1));
        rt_mutex_release(thermo);

        if(thermo_index==PROD_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(thermo_sleep);
    }
}

static void watch_enter(void* param)
{
    rt_uint32_t accl_carry_buffer[PROD_LIMIT];
    rt_uint32_t gyro_carry_buffer[PROD_LIMIT];

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

        rt_mutex_take(thermo, RT_WAITING_FOREVER);
            if(thermo_index_take<thermo_index)
            {
                pom_thermo=thermo_buffer[thermo_index_take++];
                thermo_flag=1;
            }
        rt_mutex_release(thermo);

        rt_mutex_take(gyro, RT_WAITING_FOREVER);
            if(gyro_index_take<gyro_index)
            {
                pom_gyro=gyro_buffer[gyro_index_take++];
                gyro_carry_buffer[index_carry_gyro++]=pom_gyro;
                gyro_flag=1;
            }
        rt_mutex_release(gyro);

        rt_mutex_take(accl, RT_WAITING_FOREVER);
            if(accl_index_take<thermo_index)
            {
                pom_accl=accl_buffer[accl_index_take++];
                accl_carry_buffer[index_carry_accl++]=pom_accl;
                accl_flag=1;
            }
        rt_mutex_release(accl);

        rt_mutex_take(out, RT_WAITING_FOREVER);
            if(gyro_flag==1 || thermo_flag==1 || accl_flag==1)
            {
                write_flag=1;
            }
            if(accl_flag==1 && gyro_flag==1)
            {
                sprintf(out_buffer[watch_index++],"RISK:%d:%s",pom_gyro*pom_accl,pom_gyro*pom_accl<risk_limit ? "OK" : "NOK");
            }
            if(accl_flag==1)
            {
                accl_flag=0;
                sprintf(out_buffer[watch_index++],"ACCL:%d:%s",pom_accl,pom_accl<accl_limit ? "OK" : "NOK");
                if(index_carry_gyro>1)
                {
                    for(int i=0;i<index_carry_gyro;++i)
                    {
                        sprintf(out_buffer[watch_index++],"RISK:%d:%s",gyro_carry_buffer[i]*pom_accl,gyro_carry_buffer[i]*pom_accl<risk_limit ? "OK" : "NOK");
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
                sprintf(out_buffer[watch_index++],"GYRO:%d:%s",pom_gyro,pom_gyro<gyro_limit ? "OK" : "NOK");
                if(index_carry_accl>1)
                {
                    for(int i=0;i<index_carry_accl;++i)
                    {
                        sprintf(out_buffer[watch_index++],"RISK:%d:%s",accl_carry_buffer[i]*pom_gyro,accl_carry_buffer[i]*pom_gyro<risk_limit ? "OK" : "NOK");
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
                sprintf(out_buffer[watch_index++],"THERMO:%d:%s",pom_thermo,pom_thermo>=thermo_low_limit && pom_thermo<=thermo_up_limit ? "OK" : "NOK");
            }
        rt_mutex_release(out);

        if(watch_index==4*PROD_LIMIT)
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
        rt_mutex_take(out, RT_WAITING_FOREVER);
            if(write_flag==1 || write_to_the_end==1)
            {
                rt_kprintf("%s\n",out_buffer[out_index++]);
                write_flag=0;
            }
        rt_mutex_release(out);
        if(out_index==4*PROD_LIMIT)
        {
            break;
        }
        rt_thread_mdelay(out_sleep);
    }
    rt_kprintf("Kraj");
}

int main(int argc, char **argv)
{
    accelerometer=rt_thread_create("A", accl_enter, RT_NULL, thread_stack_size, thread_prio, thread_stack_size);
    gyroscope=rt_thread_create("B", gyro_enter, RT_NULL, thread_stack_size, thread_prio, thread_stack_size);
    thermometer=rt_thread_create("C", thermo_enter, RT_NULL, thread_stack_size, thread_prio, thread_stack_size);
    output=rt_thread_create("D", out_enter, RT_NULL, thread_stack_size, thread_prio, thread_stack_size);
    watchdog=rt_thread_create("E", watch_enter, RT_NULL, thread_stack_size, thread_prio, thread_stack_size);

    accl=rt_mutex_create("a", RT_IPC_FLAG_FIFO);
    gyro=rt_mutex_create("b", RT_IPC_FLAG_FIFO);
    thermo=rt_mutex_create("c", RT_IPC_FLAG_FIFO);
    out=rt_mutex_create("d", RT_IPC_FLAG_FIFO);

    if(accl==RT_NULL)
    {
        rt_kprintf("Greska pri kreiranju mutex-a!");
        return -1;
    }

    if(gyro==RT_NULL)
    {
        rt_kprintf("Greska pri kreiranju mutex-a!");
        return -1;
    }

    if(thermo==RT_NULL)
    {
        rt_kprintf("Greska pri kreiranju mutex-a!");
        return -1;
    }

    if(out==RT_NULL)
    {
        rt_kprintf("Greska pri kreiranju mutex-a!");
        return -1;
    }

    if(accelerometer!=RT_NULL)
    {
        rt_thread_startup(accelerometer);
    }
    else
    {
        rt_kprintf("Pokretanje Accelerometer-a nije uspjelo!");
        return -1;
    }

    if(gyroscope!=RT_NULL)
    {
        rt_thread_startup(gyroscope);
    }
    else
    {
        rt_kprintf("Pokretanje Gyroscope-a nije uspjelo!");
        return -1;
    }

    if(thermometer!=RT_NULL)
    {
        rt_thread_startup(thermometer);
    }
    else
    {
        rt_kprintf("Pokretanje Thermometer-a nije uspjelo!");
        return -1;
    }
    if(output!=RT_NULL)
    {
        rt_thread_startup(output);
    }
    else
    {
        rt_kprintf("Pokretanje Output-a nije uspjelo!");
        return -1;
    }
    if(watchdog!=RT_NULL)
    {
        rt_thread_startup(watchdog);
    }
    else
    {
        rt_kprintf("Pokretanje Watchdog-a nije uspjelo!");
        return -1;
    }
    return 0;
}
