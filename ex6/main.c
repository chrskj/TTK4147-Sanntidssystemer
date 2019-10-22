#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <pthread.h>

#include "io.h"

#define TEST_A 1
#define TEST_B 2
#define TEST_C 3

// Set single CPU for pthread threads (NOT Xenomai threads!)
int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);

    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

typedef struct responseTaskArgs responseTaskArgs; 
struct responseTaskArgs {
    int channel;
};

// Note the argument and return types: void*
void task_aperiod(void* args){
    set_cpu(1);
    unsigned long duration = 10000000000;  // 10 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    responseTaskArgs a = *(responseTaskArgs*)args;
    //rt_printf("this should be channel 1: %d\n", a.channel);
    while(1){
        if(io_read(a.channel) == 0){
            io_write(a.channel, 0);
            //rt_printf("this should be 0: %d\n", io_read(a.channel));
            //printf("A should be 0 %u\n", gpio_get_pin_value(RESPONSE_A));
            rt_timer_spin(10000);
            io_write(a.channel, 1);
            //rt_printf("this should be 1: %d\n", io_read(a.channel));
        }

        if(rt_timer_read() > endTime){
        rt_printf("Time expired\n");
        rt_task_delete(NULL);
        }

        if(rt_task_yield()){
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }    
}

void task_period(void* args){
    set_cpu(1);

    unsigned long duration = 10000000000;  // 10 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    responseTaskArgs a = *(responseTaskArgs*)args;
    rt_task_set_periodic(NULL, TM_NOW, 500*1000); //500*1000
    while(1){
        if(io_read(a.channel) == 0){
            // Do periodic work
            io_write(a.channel, 0);
            rt_timer_spin(10000);
            io_write(a.channel, 1);
        } 
        if(rt_timer_read() > endTime){
        rt_printf("Time expired\n");
        rt_task_delete(NULL);
        }
        if(rt_task_yield()){
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }  
        rt_task_wait_period(NULL); 
    }
}

void* disturb(void* args){
    set_cpu(1);
    while(1){
        asm volatile("" ::: "memory");
    }  
}



int main(){
   // Call these at the start of main
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT | MCL_FUTURE);
    
    pthread_t distList[1];
    for (int i = 0; i < 10; i++){
        pthread_create(&distList[i], NULL, disturb, NULL);
    }

    // do periodic work ...
    RT_TASK task1;
    RT_TASK task2;
    RT_TASK task3;

    rt_task_create(&task1, "task 1", 0, 50, T_CPU(1));
    rt_task_create(&task2, "task 2", 0, 50, T_CPU(1));
    rt_task_create(&task3, "task 3", 0, 50, T_CPU(1));


    rt_task_start(&task1, task_period, &(responseTaskArgs){TEST_A});
    rt_task_start(&task2, task_period, &(responseTaskArgs){TEST_B});
    rt_task_start(&task3, task_period, &(responseTaskArgs){TEST_C});
    
    for (int i = 0; i < 10; i++){
        pthread_join(distList[i], NULL);
    }

    while(1)
    {

    }

    return 0;
}