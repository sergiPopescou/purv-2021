#include <rtthread.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#define THREAD_PRIORITY       6
#define THREAD_STACK_SIZE     512
#define THREAD_TIMESLICE      5

static rt_uint32_t acc_cnt=100;
static rt_uint32_t gyro_cnt=100;
static rt_uint32_t therm_cnt=100;

static rt_uint32_t acc_sim = 20;
static rt_uint32_t gyro_sim = 20;
static rt_uint32_t therm_sim = 20;

static rt_thread_t producer_acc=RT_NULL;
static rt_thread_t producer_gyro=RT_NULL;
static rt_thread_t producer_therm=RT_NULL;
static rt_thread_t consumer_watchdog=RT_NULL;
static rt_thread_t consumer_out=RT_NULL;

static int acc_end=0;
static int gyro_end=0;
static int therm_end=0;
static int out_end=0;



static rt_uint32_t acc_index,gyro_index,therm_index,out_index;
struct rt_semaphore sem_lock_acc,sem_lock_gyro,sem_lock_therm;
struct rt_semaphore sem_lock_out;
rt_uint32_t* acc_buffer;
rt_uint32_t* gyro_buffer;
rt_uint32_t* therm_buffer;
char out[400][20];

rt_uint32_t temperature = 20;
void accelerometer_thread_entry(void *parameter)
{


    rt_uint32_t iter=0;
    //rt_uint32_t cnt = 10;
    acc_buffer=(rt_uint32_t*)malloc((acc_cnt/2)*sizeof(rt_uint32_t));
    acc_index=-1;
    rt_uint32_t r=0;
    while(iter<acc_cnt)
    {
        rt_sem_take(&sem_lock_acc, RT_WAITING_FOREVER);
        r=rand()%255;
        acc_buffer[++acc_index]=r;
        rt_sem_release(&sem_lock_acc);
        iter++;
        rt_thread_mdelay(acc_sim);
    }
    while(acc_index!=-1)
          {

          }
    acc_end=1;
    free(acc_buffer);
}
void gyroscope_thread_entry(void *parameter)
{


    rt_uint32_t iter=0;
//      rt_uint32_t cnt = 10;
      gyro_index=-1;
      gyro_buffer=(rt_uint32_t*)malloc((gyro_cnt/2)*sizeof(rt_uint32_t));
      rt_uint32_t r=0;

      while(iter<gyro_cnt)
      {
          rt_sem_take(&sem_lock_gyro, RT_WAITING_FOREVER);
          r=rand()%255;
          gyro_buffer[++gyro_index]=r;
          rt_sem_release(&sem_lock_gyro);
          iter++;
          //rt_kprintf("gyro upisao %d\n",r);
          rt_thread_mdelay(gyro_sim);
      }
      while(gyro_index!=-1)
      {

      }
      gyro_end=1;
      free(gyro_buffer);
}
void thermometer_thread_entry(void *parameter)
{

    rt_uint32_t f;
    rt_uint32_t iter=0;
  //    rt_uint32_t cnt = 10;
      therm_index=-1;
      therm_buffer=(rt_uint32_t*)malloc((therm_cnt/2)*sizeof(rt_uint32_t));
      rt_uint32_t r=0;

      while(iter<therm_cnt)
      {   rt_sem_take(&sem_lock_therm, RT_WAITING_FOREVER);
          f=rand()%100;
          r=rand()%100;
          if(r<=33)
          {
              therm_buffer[++therm_index]=temperature;
          }
          else if(r>33 && r<=66 )
          {
              if(f<50)
              {
                  therm_buffer[++therm_index]=--temperature;
              }
              else
              {
                  therm_buffer[++therm_index]=++temperature;
              }
          }
          else {
            if(f<50)
            {
                temperature-=2;
                therm_buffer[++therm_index]=temperature;

            }
             else
             {
                 temperature+=2;
                therm_buffer[++therm_index]=temperature;
             }

        }
          rt_sem_release(&sem_lock_therm);
          iter++;
          rt_thread_mdelay(therm_sim);
      }
      while(therm_index!=-1)
            {

            }
          therm_end=1;
      free(therm_buffer);
}

void watchdog_thread_entry(void* parameter)
{
    uint32_t sim = rand()%5;
    out_index=-1;
    char acc[20];
    rt_uint32_t* acc_risk;
    rt_uint32_t* gyro_risk;
    acc_risk=(rt_uint32_t*)malloc((acc_cnt)*sizeof(rt_uint32_t));
    gyro_risk=(rt_uint32_t*)malloc((gyro_cnt)*sizeof(rt_uint32_t));

    rt_uint32_t risk_acc_index=-1;
    rt_uint32_t risk_gyro_index=-1;
    char gyro[20];
    char therm[20];
    char a_v[20];
    char risk[20];
    char r_v[20];
      char g_v[20];
      char t_v[20];
      rt_uint32_t i =0;
    rt_uint32_t acc_value,gyro_value,therm_value,risk_value;
    while(!gyro_end && !acc_end && !therm_end)
    {

        rt_sem_take(&sem_lock_out,RT_WAITING_FOREVER);
    if(rt_sem_take(&sem_lock_acc,RT_WAITING_NO))
    {

       if(acc_index!=-1)
       {
        strcpy(acc,"ACCL:");
        acc_value=acc_buffer[acc_index--];
        acc_risk[++risk_acc_index]=acc_value;
        itoa(acc_value,a_v,10);
        strcat(acc,a_v);
        if(acc_value<200)
            strcat(acc,":OK");
        else {
            strcat(acc,":NOK");
        }
        rt_sem_release(&sem_lock_acc);
        strcpy(out[++out_index],acc);
       }
       else
           rt_sem_release(&sem_lock_acc);
    }

    if(rt_sem_take(&sem_lock_gyro,RT_WAITING_NO))
        {
            if(gyro_index!=-1)
            {
             strcpy(gyro,"GYRO:");
             gyro_value=gyro_buffer[gyro_index--];
             gyro_risk[++risk_gyro_index]=gyro_value;
             itoa(gyro_value,g_v,10);
            strcat(gyro,g_v);
            if(gyro_value<190)
                strcat(gyro,":OK");
            else
            {
                strcat(gyro,":NOK");
            }


            rt_sem_release(&sem_lock_gyro);
            strcpy(out[++out_index],gyro);

            }
            else
            {
                rt_sem_release(&sem_lock_gyro);
            }
        }

    //kod za risk


        if(!(risk_acc_index<i) && !(risk_gyro_index<i))
        {
            strcpy(risk,"RISK:");
            risk_value = acc_risk[i]*gyro_risk[i];
            i++;
            itoa(risk_value,r_v,10);
            strcat(risk,r_v);
            if(risk_value<16000)
            {
                strcat(risk,":OK");
            }
            else
            {
                strcat(risk,"NOK");
            }
            strcpy(out[++out_index],risk);

        }




    if(rt_sem_take(&sem_lock_therm,RT_WAITING_NO))
           {
               if(therm_index!=-1)
               {
               strcpy(therm,"TEMP:");
               therm_value=therm_buffer[therm_index--];
                itoa(therm_value,t_v,10);
                strcat(therm,t_v);
               if(therm_value<=25 && therm_value>=15)
                   strcat(therm,":OK");
               else {
                   strcat(therm,":NOK");
               }
               rt_sem_release(&sem_lock_therm);
               strcpy(out[++out_index],therm);
               }
               else {
                   rt_sem_release(&sem_lock_therm);
            }
           }

    rt_sem_release(&sem_lock_out);
       rt_thread_mdelay(sim);
   }

    while(out_index!=-1)
          {

          }
    out_end=1;

}

void output_thread_entry(void* parameter)
{
    rt_uint32_t i=0;
    rt_uint32_t risk_cnt= (acc_cnt<gyro_cnt) ? acc_cnt : gyro_cnt;
   while(i<(acc_cnt+gyro_cnt+therm_cnt+risk_cnt)){

    rt_sem_take(&sem_lock_out, RT_WAITING_FOREVER);

    if(out_index!=-1)
    {
       rt_kprintf("%s \n", *(out+(i++)));
    }
    rt_sem_release(&sem_lock_out);

    rt_thread_mdelay(50);
   }
}
int main(void)
{
       rt_sem_init(&sem_lock_acc, "acc",     1,      RT_IPC_FLAG_FIFO);
       rt_sem_init(&sem_lock_gyro, "gyro",   1, RT_IPC_FLAG_FIFO);
       rt_sem_init(&sem_lock_therm, "therm",     1,      RT_IPC_FLAG_FIFO);
       rt_sem_init(&sem_lock_out, "out", 1, RT_IPC_FLAG_FIFO);
    srand(NULL);

    producer_acc=rt_thread_create("acc", accelerometer_thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if(producer_acc!=RT_NULL){
        rt_thread_startup(producer_acc);
    }

    producer_gyro=rt_thread_create("gyro", gyroscope_thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if(producer_gyro!=RT_NULL)
    {
     rt_thread_startup(producer_gyro);
    }
    producer_therm=rt_thread_create("therm", thermometer_thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if(producer_therm!=RT_NULL)
    {
           rt_thread_startup(producer_therm);
    }
    consumer_watchdog=rt_thread_create("watchdog", watchdog_thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if(consumer_watchdog!=RT_NULL)
    {
           rt_thread_startup(consumer_watchdog);
    }
    consumer_out=rt_thread_create("out", output_thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if(consumer_out!=RT_NULL)
    {
           rt_thread_startup(consumer_out);
    }



    return 0;
}

