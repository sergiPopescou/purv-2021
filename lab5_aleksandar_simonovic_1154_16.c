/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-06-09     Aleksandar       the first version
 */

//Pretprocesorske direktive
#include <rtthread.h>
#include <string.h>
#include <stdlib.h>

#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 5

//Definisanje kašnjenja za svaku nit
#define DELAY_ACCELEROMETER 20
#define DELAY_GYROSCOPE 20
#define DELAY_THERMOMETER 20
#define DELAY_OUTPUT 50

#define NUMBER_OF_THREAD_ITERATIONS 100              //Broj iteracija svake niti
#define BUFFER_SIZE 10                               //Veličina svakog bafera
#define STRING_LENGTH 20                             //Dužina stringa

//Poravnavanje stekova
ALIGN(RT_ALIGN_SIZE)

//Definisanje struktura za bafere
typedef struct bufferInt
{
    rt_uint8_t buffer[BUFFER_SIZE];                  //Niz podataka
    rt_uint8_t in;                                   //Pozicija na koju se smješta novi podatak u bafer
    rt_uint8_t out;                                  //Pozicija sa koje se čita podatak iz bafera
}BUFFER_INT;

typedef struct bufferString{
    char buffer[BUFFER_SIZE][STRING_LENGTH];          //Niz podataka
    rt_uint8_t in;                                    //Pozicija na koju se smješta novi podatak u bafer
    rt_uint8_t out;                                   //Pozicija sa koje se čita podatak iz bafera
}BUFFER_STRING;

//Definisanje handler-a za potrebne niti
static rt_thread_t handlerAccelerometer = RT_NULL;
static rt_thread_t handlerGyroscope     = RT_NULL;
static rt_thread_t handlerThermometer   = RT_NULL;
static rt_thread_t handlerWatchdog      = RT_NULL;
static rt_thread_t handlerOutput        = RT_NULL;

//Definisanje muteksa
static rt_mutex_t mutexAccelerometer = RT_NULL;        //muteks za zaštitu pristupa dijeljenoj memoriji bufferAccelerometer
static rt_mutex_t mutexGyroscope     = RT_NULL;        //muteks za zaštitu pristupa dijeljenoj memoriji bufferGyroscope
static rt_mutex_t mutexThermometer   = RT_NULL;        //muteks za zaštitu pristupa dijeljenoj memoriji bufferThermometer
static rt_mutex_t mutexOutput        = RT_NULL;        //muteks za zaštitu pristupa dijeljenoj memoriji bufferOutput
static rt_mutex_t mutexFinished      = RT_NULL;        //muteks za zaštitu pristupa dijeljenoj memoriji finishedFlag

//Definisanje semafora
static rt_sem_t semaphoreAccelerometerEmpty = RT_NULL; //Empty semafori ukazuju na broj slobodnih mjesta u odgovarajućem baferu
static rt_sem_t semaphoreAccelerometerFull  = RT_NULL; //Full semafori ukazuju na broj popunjenih mjesta u odgovarajućem baferu
static rt_sem_t semaphoreGyroscopeEmpty     = RT_NULL;
static rt_sem_t semaphoreGyroscopeFull      = RT_NULL;
static rt_sem_t semaphoreThermometerEmpty   = RT_NULL;
static rt_sem_t semaphoreThermometerFull    = RT_NULL;
static rt_sem_t semaphoreOutputEmpty        = RT_NULL;
static rt_sem_t semaphoreOutputFull         = RT_NULL;
static rt_sem_t semaphoreWatchdogFinished   = RT_NULL; //Semafor koji signalizira da je nit Watchdog završila sa radom

//Definisanje bafera
static BUFFER_INT bufferAccelerometer;
static BUFFER_INT bufferGyroscope;
static BUFFER_INT bufferThermometer;
static BUFFER_STRING bufferOutput;

//Flagovi koji se postavljaju kada senzorske niti završe sa radom
//Indeks 0 - accelerometer
//Indeks 1 - thermometer
//Indeks 2 - gyroscope
static rt_uint8_t finishedFlag[3] = {0, 0, 0};

//Funkcija za smještanje podataka u bafere koje koriste senzori
void storeData(rt_uint8_t data, BUFFER_INT* buffer)
{
    buffer->buffer[buffer->in] = data;
    buffer->in = (buffer->in + 1) % BUFFER_SIZE;
}

//Funkcija za dobijanje podataka iz bafera koje koriste senzori
rt_uint8_t readData(BUFFER_INT* buffer)
{
    rt_uint8_t index = buffer->out;
    buffer->out = (buffer->out + 1) % BUFFER_SIZE;
    return buffer->buffer[index];
}

//Funkcija koja govori da li je bafer koji koriste senzori prazan
rt_uint8_t isEmpty(BUFFER_INT* buffer)
{
    return buffer->in == buffer->out;
}

//Funkcija za smještanje podataka u izlazni bafer
void storeString(char* string, BUFFER_STRING* buffer)
{
    memset(buffer->buffer[buffer->in], 0, STRING_LENGTH*sizeof(char));
    strcpy(buffer->buffer[buffer->in], string);
    buffer->in = (buffer->in + 1) % BUFFER_SIZE;
}

//Funkcija za dobijanje podataka iz izlaznog bafera
char* readString(BUFFER_STRING* buffer)
{
    rt_uint8_t index = buffer->out;
    buffer->out = (buffer->out + 1) % BUFFER_SIZE;
    return buffer->buffer[index];
}

//Funkcija koja govori da li je izlazni bafer prazan
rt_uint8_t isOutputEmpty(BUFFER_STRING* buffer)
{
    return buffer->in == buffer->out;
}

//Tijelo niti accelerometer - senzor - proizvodjacka nit
static void accelerometer(void* parameter)
{
    srand(time(0) + 10000);                                                         //Definisanje početne vrijednosti seed-a
    rt_uint8_t accelerometerValue;
    rt_uint32_t i = 0;
    for(i = 0; i < NUMBER_OF_THREAD_ITERATIONS; i++)
    {
        accelerometerValue = rand() % 256;                                          //Generisanje slučajne vrijednosti - proizvodnja podatka
        if(rt_sem_take(semaphoreAccelerometerEmpty, RT_WAITING_FOREVER) == RT_EOK)  //Čekaj ako je bafer pun
        {
            rt_mutex_take(mutexAccelerometer, RT_WAITING_FOREVER);                  //Zauzmi muteks jer pristupaš dijeljenoj memoriji
                storeData(accelerometerValue, &bufferAccelerometer);                //Smjesti generisanu vrijednost u bafer
            rt_mutex_release(mutexAccelerometer);                                   //Oslobodi muteks

            rt_sem_release(semaphoreAccelerometerFull);                             //Signaliziraj da je u bafer unesen novi podatak
        }
        rt_thread_mdelay(DELAY_ACCELEROMETER);
    }
    rt_kprintf("Accelerometer thread finished\n");

    rt_mutex_take(mutexFinished, RT_WAITING_FOREVER);                               //Zauzmi muteks jer pristupaš dijeljenoj memoriji
    finishedFlag[0] = 1;                                                            //Postavi flag koji označava da je nit završila sa radom
    rt_mutex_release(mutexFinished);                                                //Oslobodi muteks

}
//Tijelo niti thermometer - senzor - proizvodjacka nit
static void thermometer(void* parameter)
{
    srand(time(0) + 20000);                                                         //Definisanje početne vrijednosti seed-a
    int thermometerValue = 20;                                                      //Početna vrijednost temperature je 20
    int i = 0;
    for(i = 0; i < NUMBER_OF_THREAD_ITERATIONS; i++)
    {
        thermometerValue = thermometerValue - ((rand() % 5) - 2);                   //Modifikacija temperature - proizvodnja podatka
        if(rt_sem_take(semaphoreThermometerEmpty, RT_WAITING_FOREVER) == RT_EOK)    //Čekaj ako je bafer pun
        {
            rt_mutex_take(mutexThermometer, RT_WAITING_FOREVER);                    //Zauzmi muteks jer pristupaš dijeljenoj memoriji
                storeData(thermometerValue, &bufferThermometer);                    //Smjesti generisanu vrijednost u bafer
            rt_mutex_release(mutexThermometer);                                     //Oslobodi muteks

            rt_sem_release(semaphoreThermometerFull);                               //Signaliziraj da je u bafer unesen novi podatak
        }
        rt_thread_mdelay(DELAY_THERMOMETER);
    }
    rt_kprintf("Thermometer thread finished\n");

    rt_mutex_take(mutexFinished, RT_WAITING_FOREVER);                               //Zauzmi muteks jer pristupaš dijeljenoj memoriji
    finishedFlag[1] = 1;                                                            //Postavi flag koji označava da je nit završila sa radom
    rt_mutex_release(mutexFinished);                                                //Oslobodi muteks
}

//Tijelo niti gyroscope - senzor - proizvodjacka nit
static void gyroscope(void* parameter)
{
    srand(time(0) + 30000);                                                         //Definisanje početne vrijednosti seed-a
    rt_uint8_t gyroscopeValue;
    rt_uint32_t i = 0;
    for(i = 0; i < NUMBER_OF_THREAD_ITERATIONS; i++)
    {
        gyroscopeValue = rand() % 256;                                             //Generisanje slučajne vrijednosti - proizvodnja podatka
        if(rt_sem_take(semaphoreGyroscopeEmpty, RT_WAITING_FOREVER) == RT_EOK)     //Čekaj ako je bafer pun
        {
            rt_mutex_take(mutexGyroscope, RT_WAITING_FOREVER);                     //Zauzmi muteks jer pristupaš dijeljenoj memoriji
                storeData(gyroscopeValue, &bufferGyroscope);                       //Smjesti generisanu vrijednost u bafer
            rt_mutex_release(mutexGyroscope);                                      //Oslobodi muteks

            rt_sem_release(semaphoreGyroscopeFull);                                //Signaliziraj da je u bafer unesen novi podatak
        }
        rt_thread_mdelay(DELAY_GYROSCOPE);
    }
    rt_kprintf("Gyroscope thread finished\n");

    rt_mutex_take(mutexFinished, RT_WAITING_FOREVER);                              //Zauzmi muteks jer pristupaš dijeljenoj memoriji
    finishedFlag[2] = 1;                                                           //Postavi flag koji označava da je nit završila sa radom
    rt_mutex_release(mutexFinished);                                               //Oslobodi muteks
}

//Tijelo niti watchdog - proizvođačka i potrošačka nit
static void watchdog(void* parameter)
{
    srand(time(0) + 20000);                                                             //Definisanje početne vrijednosti seed-a
    rt_uint8_t randomDelay;
    rt_uint8_t dataAccelerometer = 0;
    rt_uint8_t dataGyroscope = 0;
    rt_uint8_t dataThermometer = 0;
    rt_uint8_t dataAccelerometerOld = 0;
    rt_uint8_t dataGyroscopeOld = 0;
    rt_uint16_t productAccelerometerGyroscope = 0;                                      //Mora biti duplo većeg kapaciteta da ne dođe do odsjecanja
    char string[STRING_LENGTH];                                                         //Niz za smještanje čitavog rezultujuceg stringa
    char number[7];                                                                     //Niz za konverziju brojeva u string

    while(1)
    {
        randomDelay = rand() % 5 + 1;
        if(rt_mutex_take(mutexFinished, RT_WAITING_FOREVER) == RT_EOK)                  //Zauzmi muteks jer pristupaš dijeljenoj memoriji
        {
            if(finishedFlag[0] == 1 && finishedFlag[1] == 1 && finishedFlag[2] == 1)    //Provjeri da li su sve senzorske niti završile
            {
                if(isEmpty(&bufferAccelerometer) && isEmpty(&bufferGyroscope) && isEmpty(&bufferThermometer))
                {                                                                       //Ako su niti završile i ako su svi baferi ispražnjeni
                    rt_mutex_release(mutexFinished);                                    //Oslobodi muteks
                    rt_sem_release(semaphoreWatchdogFinished);                          //Signaliziraj da je watchdog završio sa radom
                    rt_kprintf("Watchdog thread finished\n");
                    break;                                                              //Završi sa radom
                }
            }
            rt_mutex_release(mutexFinished);                                            //Oslobodi muteks
        }

        if(rt_sem_trytake(semaphoreAccelerometerFull) == RT_EOK)                        //Provjeri da li postoji podatak u baferu
        {
            rt_mutex_take(mutexAccelerometer, RT_WAITING_FOREVER);                      //Zauzmi muteks jer pristupaš dijeljenoj memoriji
                dataAccelerometer = readData(&bufferAccelerometer);                     //Pročitaj podatak iz bafera
            rt_mutex_release(mutexAccelerometer);                                       //Oslobodi muteks

            rt_sem_release(semaphoreAccelerometerEmpty);                                //Signaliziraj da je podatak preuzet iz bafera

            memset(string, 0, STRING_LENGTH*sizeof(char));                              //Čišćenje stringa
            strcat(string, "ACCL:");                                                    //Formiranje stringa
            itoa(dataAccelerometer, number, 10);
            strcat(string, number);
            if(dataAccelerometer < 200)
                strcat(string,":OK");
            else
                strcat(string,":NOK");

            if(rt_sem_take(semaphoreOutputEmpty, RT_WAITING_FOREVER) == RT_EOK)         //Čekaj ako je bafer pun
            {
                rt_mutex_take(mutexOutput, RT_WAITING_FOREVER);                         //Zauzmi muteks jer pristupaš dijeljenoj memoriji
                    storeString(string, &bufferOutput);                                 //Smjesti string u bafer
                rt_mutex_release(mutexOutput);                                          //Oslobodi muteks

                rt_sem_release(semaphoreOutputFull);                                    //Signaliziraj da je podatak smješten u bafer
            }
        }

        if(rt_sem_trytake(semaphoreGyroscopeFull) == RT_EOK)
        {
            rt_mutex_take(mutexGyroscope, RT_WAITING_FOREVER);
                dataGyroscope = readData(&bufferGyroscope);
            rt_mutex_release(mutexGyroscope);

            rt_sem_release(semaphoreGyroscopeEmpty);

            memset(string, 0, STRING_LENGTH*sizeof(char));
            strcat(string, "GYRO:");
            itoa(dataGyroscope, number, 10);
            strcat(string, number);
            if(dataGyroscope < 190)
                strcat(string,":OK");
            else
                strcat(string,":NOK");

            if(rt_sem_take(semaphoreOutputEmpty, RT_WAITING_FOREVER) == RT_EOK)
            {
                rt_mutex_take(mutexOutput, RT_WAITING_FOREVER);
                    storeString(string, &bufferOutput);
                rt_mutex_release(mutexOutput);

                rt_sem_release(semaphoreOutputFull);
            }

        }

        if(rt_sem_trytake(semaphoreThermometerFull) == RT_EOK)
        {
            rt_mutex_take(mutexThermometer, RT_WAITING_FOREVER);
                dataThermometer = readData(&bufferThermometer);
            rt_mutex_release(mutexThermometer);

            rt_sem_release(semaphoreThermometerEmpty);

            memset(string, 0, STRING_LENGTH*sizeof(char));
            strcat(string, "TEMP:");
            itoa(dataThermometer, number, 10);
            strcat(string, number);
            if(dataThermometer <= 20 && dataThermometer >= 15)
                strcat(string,":OK");
            else
                strcat(string,":NOK");

            if(rt_sem_take(semaphoreOutputEmpty, RT_WAITING_FOREVER) == RT_EOK)
            {
                rt_mutex_take(mutexOutput, RT_WAITING_FOREVER);
                    storeString(string, &bufferOutput);
                rt_mutex_release(mutexOutput);

                rt_sem_release(semaphoreOutputFull);
            }
        }

        if (dataGyroscope != dataGyroscopeOld || dataAccelerometer != dataAccelerometerOld)     //Ako je došlo do promjene u odnosu
        {                                                                                       //na prethodnu vrijednost sa senzora
            productAccelerometerGyroscope = dataGyroscope * dataAccelerometer;                  //Računaj proizvod
            memset(string, 0, STRING_LENGTH*sizeof(char));
            strcat(string, "RISK:");
            itoa(productAccelerometerGyroscope, number, 10);
            strcat(string, number);
            if(productAccelerometerGyroscope < 16000)
                strcat(string,":OK");
            else
                strcat(string,":NOK");

            if(rt_sem_take(semaphoreOutputEmpty, RT_WAITING_FOREVER) == RT_EOK)
            {
                rt_mutex_take(mutexOutput, RT_WAITING_FOREVER);
                    storeString(string, &bufferOutput);
                rt_mutex_release(mutexOutput);

                rt_sem_release(semaphoreOutputFull);
            }
            dataAccelerometerOld = dataAccelerometer;                                           //Pamćenje prethodno iskorištenih vrijednosti
            dataGyroscopeOld = dataGyroscope;
        }
        rt_thread_mdelay(randomDelay);
    }
}


//Tijelo niti output - potrošačka nit
static void output(void *parameter)
{
    char string[STRING_LENGTH];
    while(1)
    {
        if(rt_sem_trytake(semaphoreWatchdogFinished) == RT_EOK)                    //Provjeri da li je nit watchdog završila sa radom
        {
            if(isOutputEmpty(&bufferOutput))                                       //Ako je watchdog zavrsio sa radom, provjeri da li su svi podaci
            {                                                                      //ispisani na konzolu
                rt_kprintf("Output thread finished\n");
                break;                                                             //Nit output završava sa radom
            }
            rt_sem_release(semaphoreWatchdogFinished);
        }
        if(rt_sem_take(semaphoreOutputFull, RT_WAITING_FOREVER) == RT_EOK)         //Čekaj ako je bafer prazan
        {
            rt_mutex_take(mutexOutput, RT_WAITING_FOREVER);                        //Zauzmi muteks jer pristupaš dijeljenoj memoriji
                strcpy(string, readString(&bufferOutput));                         //Pročitaj podatak iz bafera
            rt_mutex_release(mutexOutput);                                         //Oslobodi muteks
            rt_sem_release(semaphoreOutputEmpty);                                  //Signaliziraj da je podatak preuzet iz bafera
            rt_kprintf("%s\n",string);                                             //Ispiši podatak na konzolu
        }
        rt_thread_mdelay(DELAY_OUTPUT);
    }
}


int zadaca (void)
{
    //Kreiranje muteksa
    mutexAccelerometer = rt_mutex_create("Accelerometer mutex", RT_IPC_FLAG_FIFO);
    if (mutexAccelerometer == RT_NULL)
    {
        rt_kprintf("Neuspjesno kreiran Accelerometer mutex.\n");
        return -1;
    }

    mutexGyroscope = rt_mutex_create("Gyroscope mutex", RT_IPC_FLAG_FIFO);
    if (mutexGyroscope == RT_NULL)
    {
        rt_kprintf("Neuspjesno kreiran Gyroscope mutex.\n");
        return -1;
        }

    mutexThermometer = rt_mutex_create("Thermometer mutex", RT_IPC_FLAG_FIFO);
    if (mutexThermometer == RT_NULL)
    {
        rt_kprintf("Neuspjesno kreiran Thermometer mutex.\n");
        return -1;
    }

    mutexOutput = rt_mutex_create("Output mutex", RT_IPC_FLAG_FIFO);
    if (mutexOutput == RT_NULL)
    {
        rt_kprintf("Neuspjesno kreiran Output mutex.\n");
        return -1;
    }

    mutexFinished = rt_mutex_create("Finished mutex", RT_IPC_FLAG_FIFO);
    if(mutexFinished == RT_NULL)
    {
        rt_kprintf("Neuspjesno kreiran Finished mutex.\n");
        return -1;
    }
    rt_kprintf("Uspjesno kreirani muteksi\n");

    //Kreiranje semafora
    semaphoreAccelerometerEmpty = rt_sem_create("Accelerometer Empty", BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    semaphoreAccelerometerFull  = rt_sem_create("Accelerometer Full" , 0          , RT_IPC_FLAG_FIFO);
    semaphoreGyroscopeEmpty     = rt_sem_create("Gyroscope Empty"    , BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    semaphoreGyroscopeFull      = rt_sem_create("Gyroscope Full"     , 0          , RT_IPC_FLAG_FIFO);
    semaphoreThermometerEmpty   = rt_sem_create("Thermometer Empty"  , BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    semaphoreThermometerFull    = rt_sem_create("Thermometer Full"   , 0          , RT_IPC_FLAG_FIFO);
    semaphoreOutputEmpty        = rt_sem_create("Output Empty"       , BUFFER_SIZE, RT_IPC_FLAG_FIFO);
    semaphoreOutputFull         = rt_sem_create("Output Full"        , 0          , RT_IPC_FLAG_FIFO);
    semaphoreWatchdogFinished   = rt_sem_create("Watchdog finished"  , 0          , RT_IPC_FLAG_FIFO);
    rt_kprintf("Uspjesno kreirani semafori\n");

    //Kreiranje niti
    handlerAccelerometer = rt_thread_create("Accelerometer",accelerometer, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    handlerGyroscope     = rt_thread_create("Gyroscope"    ,gyroscope    , RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    handlerThermometer   = rt_thread_create("Thermometer"  ,thermometer  , RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    handlerWatchdog      = rt_thread_create("Watchdog"     ,watchdog     , RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    handlerOutput        = rt_thread_create("Output"       ,output       , RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_kprintf("Uspjesno kreirane niti\n");

    //Pokretanje niti ako su uspješno kreirane
    if (handlerAccelerometer != RT_NULL)
        rt_thread_startup(handlerAccelerometer);
    if (handlerGyroscope != RT_NULL)
        rt_thread_startup(handlerGyroscope);
    if (handlerThermometer != RT_NULL)
        rt_thread_startup(handlerThermometer);
    if (handlerWatchdog != RT_NULL)
        rt_thread_startup(handlerWatchdog);
    if (handlerOutput != RT_NULL)
        rt_thread_startup(handlerOutput);

    return 0;
}

//Eksportovanje u listu komandi msh konzole
MSH_CMD_EXPORT(zadaca, Laboratorijska vjezba 5);
