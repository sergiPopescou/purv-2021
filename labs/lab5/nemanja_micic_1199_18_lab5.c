
#include <rtthread.h>

#define THREAD_PRIORITY       6
#define THREAD_STACK_SIZE     512
#define THREAD_TIMESLICE      5

#define MAXSEM 100
#define MAX_SIZE 15
#define MAX_LENGTH 500

#define NUMBER_OF_ITERATION 100
#define SLEEP_TIME_ACCELEROMETER 20
#define SLEEP_TIME_GYROSCOPE 20
#define SLEEP_TIME_THERMOMETER 20
#define SLEEP_TIME_OUTPUT 50

rt_uint32_t array_accelerometer[MAXSEM];
rt_uint32_t array_gyroscope[MAXSEM];
rt_uint32_t array_thermometer[MAXSEM];

char array_workdog[MAX_LENGTH][MAX_SIZE];



static rt_uint32_t set_accelerometer, get_accelerometer;
static rt_uint32_t set_gyroscope, get_gyroscope;
static rt_uint32_t set_thermometer, get_termometer;

static rt_uint32_t set_workdog, get_workdog;


static rt_thread_t accelerometer_tid = RT_NULL;
static rt_thread_t gyroscope_tid = RT_NULL;
static rt_thread_t thermometer_tid = RT_NULL;

static rt_thread_t watchdog_tid = RT_NULL;
static rt_thread_t output_tid = RT_NULL;

static rt_uint32_t indicator = 0;


struct rt_semaphore sem_accelerometer_lock;
struct rt_semaphore sem__accelerometer_empty, sem_accelerometer_full;

struct rt_semaphore sem_gyroscope_lock;
struct rt_semaphore sem__gyroscope_empty, sem_gyroscope_full;

struct rt_semaphore sem_thermometer_lock;
struct rt_semaphore sem__thermometer_empty, sem_thermometer_full;

struct rt_semaphore sem_output_lock;
struct rt_semaphore sem__output_empty, sem_output_full;

//producer 1
void accelerometer_thread_entry(void *parameter)
{
    rt_uint32_t i;
    rt_uint32_t new_value;
    srand(time(RT_NULL)+1000);
    for(i = 0 ; i < NUMBER_OF_ITERATION ; i++)
    {
        rt_sem_take(&sem__accelerometer_empty, RT_WAITING_FOREVER);

        rt_sem_take(&sem_accelerometer_lock, RT_WAITING_FOREVER);

        new_value = rand() % 256;

        array_accelerometer[set_accelerometer % MAXSEM] = new_value;

        //rt_kprintf("the accelerometer generates a number: %d\n", array_accelerometer[set_accelerometer % MAXSEM]);


        set_accelerometer++;

        rt_sem_release(&sem_accelerometer_lock);

        rt_sem_release(&sem_accelerometer_full);


        rt_thread_mdelay(SLEEP_TIME_ACCELEROMETER);
    }
    rt_kprintf("the accelerometer exit!\n");
}

//producer 2
void gyroscope_thread_entry(void *parameter)
{
    rt_uint32_t i;
    rt_uint32_t new_value;
    srand(time(RT_NULL)+2000);
    for(i = 0 ; i < NUMBER_OF_ITERATION ; i++)
    {
        rt_sem_take(&sem__gyroscope_empty, RT_WAITING_FOREVER);

        rt_sem_take(&sem_gyroscope_lock, RT_WAITING_FOREVER);

        new_value = rand() % 256;

        array_gyroscope[set_gyroscope % MAXSEM] = new_value;

        //rt_kprintf("the gyroscope generates a number: %d\n", array_gyroscope[set_gyroscope % MAXSEM]);

        set_gyroscope++;

        rt_sem_release(&sem_gyroscope_lock);

        rt_sem_release(&sem_gyroscope_full);


        rt_thread_mdelay(SLEEP_TIME_GYROSCOPE);
    }
    rt_kprintf("the gyroscope exit!\n");
}


//producer 3
void thermometer_thread_entry(void *parameter)
{
    rt_uint32_t i;
    rt_uint32_t old_value;
    rt_uint32_t new_value = 20;
    rt_uint32_t adder;
    srand(time(RT_NULL)+3000);

    for(i = 0 ; i < NUMBER_OF_ITERATION ; i++)
    {
        rt_sem_take(&sem__thermometer_empty, RT_WAITING_FOREVER);

        rt_sem_take(&sem_thermometer_lock, RT_WAITING_FOREVER);

        old_value = new_value;

        adder = rand() % 5 - 2;

        new_value = old_value + adder;

        array_thermometer[set_thermometer % MAXSEM] = new_value;

        //rt_kprintf("the thermometer generates a number: %d\n", array_thermometer[set_thermometer % MAXSEM]);

        set_thermometer++;

        rt_sem_release(&sem_thermometer_lock);

        rt_sem_release(&sem_thermometer_full);


        rt_thread_mdelay(SLEEP_TIME_THERMOMETER);
    }

    rt_kprintf("the thermometer exit!\n");
}

//consumer i producer

void watchdog_thread_entry(void *parameter)
{
    rt_uint32_t sleep_time = rand() % 5 + 1;


    rt_uint32_t i;
    rt_uint32_t value;
    rt_uint32_t value_gyroscope;
    rt_uint32_t value_thermometer;


    char data_accelerometer[MAX_SIZE];
    char data_gyroscope[MAX_SIZE];
    char data_risc[MAX_SIZE];
    char data_thermometer[MAX_SIZE];



    while(1)
    {

        if(get_accelerometer == NUMBER_OF_ITERATION  && get_gyroscope == NUMBER_OF_ITERATION && get_termometer == NUMBER_OF_ITERATION)
            break;

        if(rt_sem_take(&sem_accelerometer_full,RT_WAITING_NO) == 0)
            {


            rt_sem_take(&sem_accelerometer_lock, RT_WAITING_FOREVER);



            value= array_accelerometer[get_accelerometer % MAXSEM];

            //rt_kprintf("the consumer[%d] get a number: %d\n", (get_accelerometer % MAXSEM), array_accelerometer[get_accelerometer % MAXSEM]);
            get_accelerometer++;


            rt_sem_release(&sem_accelerometer_lock);

            rt_sem_release(&sem__accelerometer_empty);

            }



        if(rt_sem_take(&sem_gyroscope_full,RT_WAITING_NO) == 0)
            {


            rt_sem_take(&sem_gyroscope_lock, RT_WAITING_FOREVER);
            value_gyroscope= array_gyroscope[get_gyroscope % MAXSEM];



            //rt_kprintf("the consumer[%d] get a number from gyroscope: %d\n", (get_gyroscope % MAXSEM), array_gyroscope[get_gyroscope % MAXSEM]);
            get_gyroscope++;


            rt_sem_release(&sem_gyroscope_lock);

            rt_sem_release(&sem__gyroscope_empty);

            }

        if(rt_sem_take(&sem_thermometer_full,RT_WAITING_NO) == 0)
            {


            rt_sem_take(&sem_thermometer_lock, RT_WAITING_FOREVER);



            value_thermometer= array_thermometer[get_termometer % MAXSEM];


            get_termometer++;


            rt_sem_release(&sem_thermometer_lock);

            rt_sem_release(&sem__thermometer_empty);
            }


        rt_sem_take(&sem__output_empty, RT_WAITING_FOREVER);

        rt_sem_take(&sem_output_lock, RT_WAITING_FOREVER);


            char snum[5];
            itoa(value,snum,10);
            strcpy(data_accelerometer,"ACCL:");
            strcat(data_accelerometer,snum);
            if(value < 200)
                strcat(data_accelerometer,":OK");
            else {
                strcat(data_accelerometer,":NOK");
            }

            strcpy(array_workdog[set_workdog % MAX_LENGTH],data_accelerometer);
            set_workdog++;

            char snumg[5];
            itoa(value_gyroscope,snumg,10);
            strcpy(data_gyroscope,"GYRO:");
            strcat(data_gyroscope,snumg);
            if(value_gyroscope < 190)
                   strcat(data_gyroscope,":OK");
            else {
                   strcat(data_gyroscope,":NOK");
                  }

            strcpy(array_workdog[set_workdog % MAX_LENGTH],data_gyroscope);

            set_workdog++;

            rt_uint32_t old_value_gyroscope;
            rt_uint32_t old_value_accelerometer;
            rt_uint32_t risc_value;

            if(value != 0 && value_gyroscope !=0 && old_value_accelerometer == 0 && old_value_gyroscope == 0)
                risc_value = value * value_gyroscope;
            if(value == 0 && value_gyroscope != 0)
            {
                old_value_gyroscope = value_gyroscope;
                risc_value =value_gyroscope;
            }
            if(value != 0 && value_gyroscope == 0)
              {
                            old_value_accelerometer = value;
                            risc_value =value;
              }
            if(old_value_accelerometer !=0 && value_gyroscope != 0)
            {
                risc_value = old_value_accelerometer * value_gyroscope;
                old_value_accelerometer = 0;
            }
            if(old_value_gyroscope != 0 && value != 0)
            {
                risc_value = old_value_gyroscope * value;
                old_value_gyroscope = 0;
            }

            char snumr[5];
            itoa(risc_value,snumr,10);
            strcpy(data_risc,"RISC:");
            strcat(data_risc,snumr);

            if(risc_value < 16000)
                strcat(data_risc,":OK");
            else {
                strcat(data_risc,":NOK");
            }

            strcpy(array_workdog[set_workdog % MAX_LENGTH], data_risc);
            set_workdog++;

            char snumt[5];

            itoa(value_thermometer,snumt,10);
            strcpy(data_thermometer,"TEMP:");
            strcat(data_thermometer,snumt);

            if(value_thermometer > 14 && value_thermometer < 26)
                strcat(data_thermometer,":OK");
            else {
                strcat(data_thermometer,":NOK");
            }

            strcpy(array_workdog[set_workdog % MAX_LENGTH], data_thermometer);
            set_workdog++;


        rt_sem_release(&sem_output_lock);

        rt_sem_release(&sem_output_full);


        rt_thread_mdelay(sleep_time);
    }

    rt_kprintf("the consumer exit!\n");
}

//producer

void output_thread_entry(void *parameter)
{
    while(1)
    {

        if(rt_sem_take(&sem_output_full,RT_WAITING_NO) == 0)
        {


            rt_sem_take(&sem_output_lock, RT_WAITING_FOREVER);



            rt_kprintf(" %s\n", array_workdog[get_workdog % MAX_LENGTH]);
            get_workdog++;


            indicator++;

            rt_sem_release(&sem_output_lock);

            rt_sem_release(&sem__output_empty);
        }

        else
            break;

        rt_thread_mdelay(SLEEP_TIME_OUTPUT);
    }
    rt_kprintf("the output finished\n");
}

int producer_consumer(void)
{
    set_accelerometer = 0;
    get_accelerometer = 0;

    set_gyroscope = 0;
    set_accelerometer = 0;

    set_thermometer = 0;
    get_termometer = 0;

    rt_sem_init(&sem_accelerometer_lock, "lock",     1,      RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem__accelerometer_empty, "empty",   MAXSEM, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_accelerometer_full, "full",     0,      RT_IPC_FLAG_FIFO);

    rt_sem_init(&sem_gyroscope_lock, "lock",     1,      RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem__gyroscope_empty, "empty",   MAXSEM, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_gyroscope_full, "full",     0,      RT_IPC_FLAG_FIFO);

    rt_sem_init(&sem_thermometer_lock, "lock",     1,      RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem__thermometer_empty, "empty",   MAXSEM, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_thermometer_full, "full",     0,      RT_IPC_FLAG_FIFO);

    rt_sem_init(&sem_output_lock, "lock",     1,      RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem__output_empty, "empty",   MAXSEM, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_output_full, "full",     0,      RT_IPC_FLAG_FIFO);



    accelerometer_tid = rt_thread_create("accelerometer",
                                    accelerometer_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY - 2, THREAD_TIMESLICE);
    if (accelerometer_tid != RT_NULL)
        rt_thread_startup(accelerometer_tid);


    gyroscope_tid = rt_thread_create("gyroscope",
                                        gyroscope_thread_entry, RT_NULL,
                                        THREAD_STACK_SIZE,
                                        THREAD_PRIORITY - 2, THREAD_TIMESLICE);
        if (gyroscope_tid != RT_NULL)
            rt_thread_startup(gyroscope_tid);

     thermometer_tid = rt_thread_create("thermometer",
                                                thermometer_thread_entry, RT_NULL,
                                                THREAD_STACK_SIZE,
                                                THREAD_PRIORITY - 2, THREAD_TIMESLICE);
         if (thermometer_tid != RT_NULL)
                    rt_thread_startup(thermometer_tid);

    watchdog_tid = rt_thread_create("watchdog",
                                    watchdog_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (watchdog_tid != RT_NULL)
        rt_thread_startup(watchdog_tid);

    output_tid = rt_thread_create("output",
                                        output_thread_entry, RT_NULL,
                                        THREAD_STACK_SIZE,
                                        THREAD_PRIORITY + 3, THREAD_TIMESLICE);
        if (output_tid != RT_NULL)
            rt_thread_startup(output_tid);

    return 0;
}

MSH_CMD_EXPORT(producer_consumer, producer_consumer sample);
