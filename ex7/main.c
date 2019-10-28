#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <pthread.h>

// Set single CPU for pthread threads (NOT Xenomai threads!)
int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);

    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Note the argument and return types: void*
void func(){
    set_cpu(1);
    while(1){}    
}

int main(){
   // Call these at the start of main
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT | MCL_FUTURE);

    // do periodic work ...
    RT_TASK task1;
    RT_TASK task2;

    rt_task_shadow(NULL, "main task", 50, T_CPU(1));
    rt_task_create(&task1, "task 1", 0, 25, T_CPU(1));
    rt_task_create(&task2, "task 2", 0, 50, T_CPU(1));
    rt_task_create(&task3, "task 3", 0, 75, T_CPU(1));

    rt_task_start(&task1, func, NULL);
    rt_task_start(&task2, func, NULL);
    rt_task_start(&task3, func, NULL);

    while(1){}
    return 0;
}
