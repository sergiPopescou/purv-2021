/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-06-02     sajin       the first version
 */

#include <rtthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5
#define MAX_BUFFER_SIZE         256
#define MAX_NUMBER_LENGHT       8

// used strings
#define ACCL "ACCL:"
#define GYRO "GYRO:"
#define TEMP "TEMP:"
#define RISK "RISK:"
#define OK ":OK"
#define NOK ":NOK"

// align stack
ALIGN(RT_ALIGN_SIZE)

// structure for passing parameters to thread
typedef struct params {
    rt_uint16_t num_of_iterations;
    rt_uint16_t sleep;
    rt_uint16_t rand_start;        // add this value on time(RT_NULL) in order to get different random values for each thread
} Param;

// structure which represents buffer
typedef struct buffer {
    rt_uint8_t head;
    rt_uint8_t tail;
    rt_uint8_t data[MAX_BUFFER_SIZE];
} Buffer;

// structure which represents string buffer
typedef struct string_buffer {
    rt_uint8_t head;
    rt_uint8_t tail;
    char* data[MAX_BUFFER_SIZE];
} StringBuffer;

void addToBuffer(Buffer*, rt_uint8_t num); // add new number to buffer
rt_uint8_t getFromBuffer(Buffer*); // get number from buffer
rt_uint8_t isEmpty(Buffer*); // check if buffer is empty
void addStringToBuffer(StringBuffer*, char* str); // add string to string buffer
void getStringFromBuffer(StringBuffer*, char** dest); // get string from string buffer
rt_uint8_t isStringBufferEmpty(StringBuffer*); // check if string buffer is empty
void watchdogCheck(rt_uint16_t data, rt_uint8_t str_size, const char* type, const char* ok_nok); // watchdog check data

// sensor's buffers
static Buffer accl_buff;
static Buffer gyro_buff;
static Buffer temp_buff;

// output buffer
static StringBuffer output_buff;

// thread finish indicator
static rt_uint8_t finish_ind[3] = {0, 0, 0};

// buffer access mutex
static rt_mutex_t accl_buff_access = RT_NULL;
static rt_mutex_t gyro_buff_access = RT_NULL;
static rt_mutex_t temp_buff_access = RT_NULL;
static rt_mutex_t out_buff_access = RT_NULL;

// semaphores
struct rt_semaphore accl_sem_empty, accl_sem_full;
struct rt_semaphore gyro_sem_empty, gyro_sem_full;
struct rt_semaphore temp_sem_empty, temp_sem_full;
struct rt_semaphore out_sem_empty, out_sem_full;
struct rt_semaphore finished_sem, watchdog_finished_sem;

// accelerometer thread
void accelerometer(void* pParam) {

    Param* params = (Param*) pParam;
    srand(time(RT_NULL) + params->rand_start);

    for (int i = 0; i < params->num_of_iterations; ++i) {

        rt_uint8_t new_acc = rand() % 256;

        rt_sem_take(&accl_sem_empty, RT_WAITING_FOREVER); // wait for semaphore
        rt_mutex_take(accl_buff_access, RT_WAITING_FOREVER); // lock mutex

        addToBuffer(&accl_buff, new_acc);

        rt_mutex_release(accl_buff_access); // release mutex
        rt_sem_release(&accl_sem_full); // signal semaphore

        rt_thread_mdelay(params->sleep);
    }

    // indicate that this thread has finished job
    rt_sem_take(&finished_sem, RT_WAITING_FOREVER);
    finish_ind[0] = 1;
    rt_sem_release(&finished_sem);
}

// gyroscope thread
void gyroscope(void* pParam) {

    Param* params = (Param*) pParam;
    srand(time(RT_NULL) + params->rand_start);

    for (int i = 0; i < params->num_of_iterations; ++i) {

        rt_uint8_t new_gyr = rand() % 256;

        rt_sem_take(&gyro_sem_empty, RT_WAITING_FOREVER); // wait for semaphore
        rt_mutex_take(gyro_buff_access, RT_WAITING_FOREVER); // lock mutex

        addToBuffer(&gyro_buff, new_gyr);

        rt_mutex_release(gyro_buff_access); // release mutex
        rt_sem_release(&gyro_sem_full); // signal semaphore

        rt_thread_mdelay(params->sleep);
    }

    // indicate that this thread has finished job
    rt_sem_take(&finished_sem, RT_WAITING_FOREVER);
    finish_ind[1] = 1;
    rt_sem_release(&finished_sem);
}

// thermometer thread
void thermometer(void* pParam) {

    Param* params = (Param*) pParam;
    srand(time(RT_NULL) + params->rand_start);
    rt_uint8_t temp = 20;

    for (int i = 0; i < params->num_of_iterations; ++i) {

        rt_sem_take(&temp_sem_empty, RT_WAITING_FOREVER); // wait for semaphore
        rt_mutex_take(temp_buff_access, RT_WAITING_FOREVER); // lock mutex

        addToBuffer(&temp_buff, temp);

        rt_mutex_release(temp_buff_access); // release mutex
        rt_sem_release(&temp_sem_full); // signal semaphore

        rt_thread_mdelay(params->sleep);

        temp += rand() % 5 - 2;
    }

    // indicate that this thread has finished job
    rt_sem_take(&finished_sem, RT_WAITING_FOREVER);
    finish_ind[2] = 1;
    rt_sem_release(&finished_sem);
}

void watchdog(void* pParam) {

    rt_uint16_t rand_start = *((rt_uint16_t*) pParam);
    srand(time(RT_NULL) + rand_start);

    // initial values
    rt_uint8_t accl_new = 0, accl_last = 0;
    rt_uint8_t gyro_new = 0, gyro_last = 0;

    while (1) {

        if (rt_sem_take(&finished_sem, RT_WAITING_NO) == RT_EOK) {

            if (finish_ind[0] == 1 && finish_ind[1] == 1 && finish_ind[2] == 1) {
                // all threads are finished
                // safe access to buffer
                if (isEmpty(&accl_buff) && isEmpty(&gyro_buff) && isEmpty(&temp_buff)) {
                    rt_sem_release(&finished_sem);
                    rt_sem_release(&watchdog_finished_sem);
                    break;
                }
            }

            rt_sem_release(&finished_sem);
        }

        // check accelerator data
        if (rt_sem_take(&accl_sem_full, RT_WAITING_NO) == RT_EOK) {

            rt_mutex_take(accl_buff_access, RT_WAITING_FOREVER); // lock mutex
            rt_uint8_t read_accl = getFromBuffer(&accl_buff);
            rt_mutex_release(accl_buff_access); // release mutex

            rt_sem_release(&accl_sem_empty); // signal semaphore

            // ok_nok string
            // here is stored OK or NOK text
            char* ok_nok = (char*) calloc(strlen(NOK), sizeof(char));
            rt_uint8_t str_size = 1;
            if (read_accl < 200) {
                // valid
                str_size += strlen(OK);
                strcat(ok_nok, OK); // add OK to string
            } else {
                // invalid
                str_size += strlen(NOK);
                strcat(ok_nok, NOK); // add NOK to string
            }

            watchdogCheck(read_accl, str_size, ACCL, ok_nok);
            free(ok_nok); // free memory

            accl_new = read_accl; // save last value
        }

        // check gyroscope data
        if (rt_sem_take(&gyro_sem_full, RT_WAITING_NO) == RT_EOK) {

            rt_mutex_take(gyro_buff_access, RT_WAITING_FOREVER); // lock mutex
            rt_uint8_t read_gyro = getFromBuffer(&gyro_buff);
            rt_mutex_release(gyro_buff_access); // release mutex

            rt_sem_release(&gyro_sem_empty); // signal semaphore

            // ok_nok string
            // here is stored OK or NOK text
            char* ok_nok = (char*) calloc(strlen(NOK), sizeof(char));
            rt_uint8_t str_size = 1; // size
            if (read_gyro < 190) {
                // valid
                str_size += strlen(OK);
                strcat(ok_nok, OK); // add OK to string
            } else {
                // invalid
                str_size += strlen(NOK);
                strcat(ok_nok, NOK); // add NOK to string
            }

            watchdogCheck(read_gyro, str_size, GYRO, ok_nok);
            free(ok_nok); // free memory

            gyro_new = read_gyro; // save last value
        }

        // check thermometer data
        if (rt_sem_take(&temp_sem_full, RT_WAITING_NO) == RT_EOK) {

            rt_mutex_take(temp_buff_access, RT_WAITING_FOREVER); // lock mutex
            rt_uint8_t read_temp = getFromBuffer(&temp_buff);
            rt_mutex_release(temp_buff_access); // release mutex

            rt_sem_release(&temp_sem_empty); // signal semaphore

            // ok_nok string
            // here is stored OK or NOK text
            char* ok_nok = (char*) calloc(strlen(NOK), sizeof(char));
            rt_uint8_t str_size = 1; // size
            if (read_temp >= 15 && read_temp <= 25) {
                // valid
                str_size += strlen(OK);
                strcat(ok_nok, OK); // add OK to string
            } else {
                // invalid
                str_size += strlen(NOK);
                strcat(ok_nok, NOK); // add NOK to string
            }

            watchdogCheck(read_temp, str_size, TEMP, ok_nok);
            free(ok_nok); // free memory
        }

        if (gyro_last != gyro_new || accl_last != accl_new) {
            // there are new values
            rt_uint16_t risk = accl_new * gyro_new;

            char* ok_nok = (char*) calloc(strlen(NOK), sizeof(char));
            rt_uint8_t str_size = 1;
            if (risk < 16000) {
                // valid
                str_size += strlen(OK);
                strcat(ok_nok, OK);
            } else {
                // invalid
                str_size += strlen(NOK);
                strcat(ok_nok, NOK);
            }

            watchdogCheck(risk, str_size, RISK, ok_nok);
            free(ok_nok);

            // this variables are used for preventing multiple calculation for same values
            accl_last = accl_new;
            gyro_last = gyro_new;
        }

        rt_uint8_t sleep = rand() % 5 + 1;
        rt_thread_mdelay(sleep);
    }
}

void output(void* pParam) {

    rt_uint16_t sleep = *((rt_uint16_t*) pParam);

    while (1) {

        if (rt_sem_take(&watchdog_finished_sem, RT_WAITING_NO) == RT_EOK) {
            // access to StringBuffer is safe, because watchdog (producer) theread is finished
            if (isStringBufferEmpty(&output_buff))
                break;
            rt_sem_release(&watchdog_finished_sem);
        }

        rt_sem_take(&out_sem_full, RT_WAITING_FOREVER); // wait for a semaphore
        rt_mutex_take(out_buff_access, RT_WAITING_FOREVER); // lock mutex

        char *res; // string
        getStringFromBuffer(&output_buff, &res); // get string from buffer

        rt_mutex_release(out_buff_access); // release mutex
        rt_sem_release(&out_sem_empty);

        rt_kprintf("%s\n", res);
        free(res); // free memory

        rt_thread_mdelay(sleep);
    }
}

// main function
int main_function(void) {

    // initialize mutexe's
    // accelerator buffer mutex
    accl_buff_access = rt_mutex_create("Accelerator buffer mutex", RT_IPC_FLAG_FIFO);
    if (accl_buff_access == RT_NULL) {
        rt_kprintf("Can't create accelerator buffer mutex.\n");
        return -1;
    }

    // gyroscope buffer mutex
    gyro_buff_access = rt_mutex_create("Gyroscope buffer mutex", RT_IPC_FLAG_FIFO);
    if (gyro_buff_access == RT_NULL) {
        rt_kprintf("Can't create gyroscope buffer mutex.\n");
        return -1;
    }

    // thermometer buffer mutex
    temp_buff_access = rt_mutex_create("Thermometer buffer mutex", RT_IPC_FLAG_FIFO);
    if (temp_buff_access == RT_NULL) {
        rt_kprintf("Can't create thermometer buffer mutex.\n");
        return -1;
    }

    // output buffer mutex
    out_buff_access = rt_mutex_create("Output buffer mutex", RT_IPC_FLAG_FIFO);
    if (out_buff_access == RT_NULL) {
        rt_kprintf("Can't create output buffer mutex.\n");
        return -1;
    }

    // semaphore initialization
    rt_sem_init(&accl_sem_empty, "Accelerator Empty Semaphore", MAX_BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    rt_sem_init(&accl_sem_full, "Accelerator Full Semaphore", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&gyro_sem_empty, "Gyroscope Empty Semaphore", MAX_BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    rt_sem_init(&gyro_sem_full, "Gyroscope Full Semaphore", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&temp_sem_empty, "Thermometer Empty Semaphore", MAX_BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    rt_sem_init(&temp_sem_full, "Thermometer Full Semaphore", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&out_sem_empty, "Output Empty Semaphore", MAX_BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    rt_sem_init(&out_sem_full, "Output Full Semaphore", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&finished_sem, "Finish Indicator Semaphore", 1, RT_IPC_FLAG_FIFO);
    rt_sem_init(&watchdog_finished_sem, "Watchdog Finished Semaphore", 0, RT_IPC_FLAG_FIFO);

    // define parameters for each thread
    // accelerator parameters
    Param* accl_params = (Param*) malloc(sizeof(Param));
    accl_params->num_of_iterations = (rt_uint16_t) 100;
    accl_params->sleep = (rt_uint16_t) 20;
    accl_params->rand_start = (rt_uint16_t) 0;

    // gyroscope parameters
    Param* gyro_params = (Param*) malloc(sizeof(Param));
    gyro_params->num_of_iterations = (rt_uint16_t) 100;
    gyro_params->sleep = (rt_uint16_t) 20;
    gyro_params->rand_start = (rt_uint16_t) 1000;

    // thermometer parameters
    Param* temp_params = (Param*) malloc(sizeof(Param));
    temp_params->num_of_iterations = (rt_uint16_t) 100;
    temp_params->sleep = (rt_uint16_t) 20;
    temp_params->rand_start = (rt_uint16_t) 2000;

    // watchdog parameter
    rt_uint16_t wcdg_rand_start = (rt_uint16_t) 500;

    // output parameter
    rt_uint16_t outp_sleep = (rt_uint16_t) 50;

    // define threads
    rt_thread_t accl_thread = RT_NULL;
    rt_thread_t gyro_thread = RT_NULL;
    rt_thread_t temp_thread = RT_NULL;
    rt_thread_t wcdg_thread = RT_NULL;
    rt_thread_t outp_thread = RT_NULL;

    // create accelerometer thread
    accl_thread = rt_thread_create(
            "Accelerometer", accelerometer, (void*) accl_params,
            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    // create gyroscope thread
    gyro_thread = rt_thread_create(
            "Gyroscope", gyroscope, (void*) gyro_params,
            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    // create thermometer thread
    temp_thread = rt_thread_create(
            "Thermometer", thermometer, (void*) temp_params,
            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    // create watch-dog thread
    wcdg_thread = rt_thread_create(
            "Watchdog", watchdog, (void*) &wcdg_rand_start,
            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    // create output thread
    outp_thread = rt_thread_create(
            "Output", output, (void*) &outp_sleep,
            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);


    // start accelerometer thread
    if (accl_thread != RT_NULL)
        rt_thread_startup(accl_thread);

    // start gyroscope thread
    if (gyro_thread != RT_NULL)
        rt_thread_startup(gyro_thread);

    // start thermometer thread
    if (temp_thread != RT_NULL)
        rt_thread_startup(temp_thread);

    // start watchdog thread
    if (wcdg_thread != RT_NULL)
        rt_thread_startup(wcdg_thread);

    // start output thread
    if (outp_thread != RT_NULL)
        rt_thread_startup(outp_thread);

    return 0;
}

// implementation
//
void addToBuffer(Buffer* buffer, rt_uint8_t d) {
    buffer->data[buffer->head] = d;
    buffer->head = (buffer->head + 1) % MAX_BUFFER_SIZE;
}

// get number from buffer
rt_uint8_t getFromBuffer(Buffer* buffer) {
    rt_uint8_t d = buffer->data[buffer->tail];
    buffer->tail = (buffer->tail + 1) % MAX_BUFFER_SIZE;
    return d;
}

rt_uint8_t isEmpty(Buffer* buffer) {
    if (buffer->head == buffer->tail)
        return 1;
    return 0;
}

// add string to string buffer
void addStringToBuffer(StringBuffer* buffer, char* s) {
    buffer->data[buffer->head] = (char*) calloc(strlen(s) + 1, sizeof(char));
    strcpy(buffer->data[buffer -> head], s);
    buffer->head = (buffer->head + 1) % 256;
}

// get string from string buffer
void getStringFromBuffer(StringBuffer* buffer, char** s) {
    *s = (char*) calloc(strlen(buffer->data[buffer->tail]) + 1, sizeof(char)); // alocate size for string
    strcpy(*s, buffer->data[buffer->tail]); // copy string from buffer to variable
    free(buffer->data[buffer->tail]); // free memory
    buffer->tail = (buffer->tail + 1) % MAX_BUFFER_SIZE; // incerement tail
}

rt_uint8_t isStringBufferEmpty(StringBuffer* buffer) {
    if (buffer->head == buffer->tail)
        return 1;
    return 0;
}

void watchdogCheck(rt_uint16_t data, rt_uint8_t str_size, const char* type, const char* ok_nok) {
    // helper string's
    char* int_str = (char*) calloc(MAX_NUMBER_LENGHT, sizeof(char));

    itoa(data, int_str, 10); // convert int to string

    str_size += strlen(int_str) + strlen(type);

    char* res = (char*) calloc(str_size, sizeof(char));
    strcat(res, type);
    strcat(res, int_str);
    strcat(res, ok_nok);

    // free memory
    free(int_str);

    rt_sem_take(&out_sem_empty, RT_WAITING_FOREVER); // take semaphore
    rt_mutex_take(out_buff_access, RT_WAITING_FOREVER); // lock mutex

    addStringToBuffer(&output_buff, res);

    rt_mutex_release(out_buff_access); // release mutex
    rt_sem_release(&out_sem_full);

    free(res); // free memory
}

MSH_CMD_EXPORT(main_function, Lab 5);
