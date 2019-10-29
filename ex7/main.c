#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <sys/mman.h>
#include <pthread.h>

#define PERIOD 100000

//Tasks A and B
RT_SEM sem;
RT_SEM semB;

//Task C
RT_MUTEX mutex;

//Task D and E
RT_MUTEX mutexA;
RT_MUTEX mutexB;


// Set single CPU for pthread threads (NOT Xenomai threads!)

void busy_wait_us(unsigned long delay){
	for(; delay > 0; delay--){
		rt_timer_spin(1000);
	}
}

int set_cpu(int cpu_number){
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);

    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

//Task A
// Note the argument and return types: void*
void function_sync(){
    set_cpu(1);
	rt_timer_spin(100*1000*1000);
	rt_printf("broadcast semaphore \n");
	rt_sem_broadcast(&sem);
	rt_printf("broadcast complete \n");
	rt_timer_spin(100*1000*1000);
    rt_sem_delete(&sem);
}

void func(){
    set_cpu(1);
	rt_printf("locking semaphore \n");
	rt_sem_p(&sem, TM_INFINITE);
	rt_printf("semaphore unlocked \n");
}

void TaskA(){
	RT_TASK task1;
    RT_TASK task2;
 	RT_TASK task3;
	
	//creation of the semaphore	
	rt_sem_create(&sem, "semaphore", 0, S_FIFO);
    rt_printf("semaphore created \n");
	// make main a Xenomai task

    rt_task_create(&task1, "task 1", 0, 25, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task2, "task 2", 0, 50, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task3, "task 3", 0, 75, T_CPU(1) | T_JOINABLE);
 	rt_printf("tasks created \n\n");

    rt_task_start(&task1, func, NULL);
    rt_task_start(&task2, func, NULL);
    rt_task_start(&task3, function_sync, NULL);
	rt_printf("tasks started \n");

	rt_task_join(&task1);
	rt_task_join(&task2);
	rt_task_join(&task3);
    
    rt_printf("end \n");	
}

//Task B
void func_low(){
    set_cpu(1);	
	rt_printf("L waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);

	rt_sem_p(&semB, TM_INFINITE);
	rt_printf("L Locked semaphore \n");

	rt_printf("L busy wait... 3 time units \n");
	busy_wait_us(3 * PERIOD);

	rt_printf("L semaphore unlocked \n");
	rt_sem_v(&semB);
}

void func_medium(){
    set_cpu(1);
	rt_printf("M waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);
	
	rt_printf("M sleep 1 time unit \n");
	rt_task_sleep(1 * PERIOD);

	rt_printf("M busy wait... 5 time units \n");
	busy_wait_us(5 * PERIOD);
}

void func_high(){
    set_cpu(1);
	rt_printf("H waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);
	
	rt_printf("H sleep 2 time units \n");
	rt_task_sleep(2 * PERIOD);

	rt_sem_p(&semB, TM_INFINITE);
	rt_printf("H Locked semaphore \n");

	rt_printf("H busy wait... 2 time units \n");
	busy_wait_us(2 * PERIOD);

	rt_printf("H semaphore unlocked \n");
	rt_sem_v(&semB);
}

void TaskB(){
	RT_TASK task_low;
    RT_TASK task_medium;
 	RT_TASK task_high;
	RT_TASK task_sync;
	
	//creation of the semaphore	
	rt_sem_create(&sem, "semaphore", 0, S_FIFO);
	rt_sem_create(&semB, "semaphore B" , 1, S_FIFO);
    rt_printf("semaphore created \n");


    rt_task_create(&task_low, "task low", 0, 25, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_medium, "task medium", 0, 50, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_high, "task high", 0, 75, T_CPU(1) | T_JOINABLE);
	rt_task_create(&task_sync, "task sync", 0, 20, T_CPU(1) | T_JOINABLE);
 	rt_printf("tasks created \n\n");

	rt_task_start(&task_sync, function_sync, NULL);
    rt_task_start(&task_low, func_low, NULL);
    rt_task_start(&task_medium, func_medium, NULL);
 	rt_task_start(&task_high, func_high, NULL);
	rt_printf("tasks started \n");

	rt_task_join(&task_low);
	rt_task_join(&task_medium);
	rt_task_join(&task_high);
	rt_task_join(&task_sync);
    
    rt_printf("end \n");	
}


//Task C
void func_low_taskC(){
    set_cpu(1);	
	rt_printf("L waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);

	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("L Locked mutex \n");

	rt_printf("L busy wait... 3 time units \n");
	busy_wait_us(3 * PERIOD);
	rt_printf("L completed \n");

	rt_printf("L mutex unlocked \n");
	rt_mutex_release(&mutex);
}

void func_medium_taskC(){
    set_cpu(1);
	rt_printf("M waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);
	
	rt_printf("M sleep 1 time unit \n");
	rt_task_sleep(1 * PERIOD);

	rt_printf("M busy wait... 5 time units \n");
	busy_wait_us(5 * PERIOD);
	rt_printf("M completed \n");
}

void func_high_taskC(){
    set_cpu(1);
	rt_printf("H waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);
	
	rt_printf("H sleep 2 time units \n");
	rt_task_sleep(2 * PERIOD);

	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("H Locked mutex \n");

	rt_printf("H busy wait... 2 time units \n");
	busy_wait_us(2 * PERIOD);
	rt_printf("H completed \n");

	rt_printf("H mutex unlocked \n");
	rt_mutex_release(&mutex);
}

void TaskC(){
	RT_TASK task_low;
    RT_TASK task_medium;
 	RT_TASK task_high;
	RT_TASK task_sync;
	
	//creation of the mutex
	rt_sem_create(&sem, "semaphore", 0, S_FIFO);
	rt_mutex_create(&mutex, "mutex");
    rt_printf("semaphore and mutex created \n");


    rt_task_create(&task_low, "task low", 0, 25, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_medium, "task medium", 0, 50, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_high, "task high", 0, 75, T_CPU(1) | T_JOINABLE);
	rt_task_create(&task_sync, "task sync", 0, 20, T_CPU(1) | T_JOINABLE);
 	rt_printf("tasks created \n\n");

	rt_task_start(&task_sync, function_sync, NULL);
    rt_task_start(&task_low, func_low_taskC, NULL);
    rt_task_start(&task_medium, func_medium_taskC, NULL);
 	rt_task_start(&task_high, func_high_taskC, NULL);
	rt_printf("tasks started \n");

	rt_task_join(&task_low);
	rt_task_join(&task_medium);
	rt_task_join(&task_high);
	rt_task_join(&task_sync);
    rt_printf("end \n");	
}

//Task D
void func_low_taskD(){
    set_cpu(1);	
	rt_printf("L waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);

	rt_mutex_acquire(&mutexA, TM_INFINITE);
	rt_printf("L Locked mutexA \n");

	rt_printf("L busy wait... 3 time units (A) \n");
	busy_wait_us(3 * PERIOD);
	rt_printf("L completed (A) \n");

	rt_mutex_acquire(&mutexB, TM_INFINITE);
	rt_printf("L Locked mutexB \n");

	rt_printf("L busy wait... 3 time units (B) \n");
	busy_wait_us(3 * PERIOD);
	rt_printf("L completed (B) \n");

	rt_printf("L mutexB unlocked \n");
	rt_mutex_release(&mutexB);

	rt_printf("L mutexA unlocked \n");
	rt_mutex_release(&mutexA);

	rt_printf("L busy wait... 1 time unit \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("L completed \n");
}

void func_high_taskD(){
    set_cpu(1);
	rt_printf("H waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);

	rt_printf("H sleep 1 time unit (B) \n");
	rt_task_sleep(1 * PERIOD);

	rt_mutex_acquire(&mutexB, TM_INFINITE);
	rt_printf("H Locked mutexB \n");

	rt_printf("H busy wait... 1 time unit (B) \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("H completed (B) \n");

	rt_mutex_acquire(&mutexA, TM_INFINITE);
	rt_printf("H Locked mutexA \n");

	rt_printf("H busy wait... 2 time units (A) \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("H completed (A) \n");

	rt_printf("H mutexA unlocked \n");
	rt_mutex_release(&mutexA);

	rt_printf("H mutexB unlocked \n");
	rt_mutex_release(&mutexB);

	rt_printf("H busy wait... 1 time unit \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("H completed \n");
}

void TaskD(){
	RT_TASK task_low;
 	RT_TASK task_high;
	RT_TASK task_sync;
	
	//creation of the mutex
	rt_sem_create(&sem, "semaphore", 0, S_FIFO);
	rt_mutex_create(&mutexA, "mutexA");
	rt_mutex_create(&mutexB, "mutexB");
    rt_printf("semaphore and mutex created \n");


    rt_task_create(&task_low, "task low", 0, 25, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_high, "task high", 0, 75, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_sync, "task sync", 0, 20, T_CPU(1) | T_JOINABLE);
 	rt_printf("tasks created \n\n");

	rt_task_start(&task_sync, function_sync, NULL);
    rt_task_start(&task_low, func_low_taskD, NULL);
    rt_task_start(&task_high, func_high_taskD, NULL);
	rt_printf("tasks started \n");

	rt_task_join(&task_low);
	rt_task_join(&task_high);
	rt_task_join(&task_sync);
    rt_printf("end \n");	
}

//Task E

struct resource {
	struct {
		RT_MUTEX* mutex;
		int mutex_prio;
	} resource;
};

struct task_t {
	struct {
		int base_prio;
		int boost_prio;
	} task_t;
};

void func_low_taskE();
void func_high_taskE();

void iccp_lock(struct resource* r, struct task_t* t){
	rt_mutex_acquire(r->mutex, TM_INFINITE);
	rt_task_set_priority(NULL, t->boost_prio);
};

void iccp_unlock(struct resource* r, struct task_t* t){
	rt_task_set_priority(NULL, t->base_prio);
	rt_mutex_release(r->mutex);
};

void TaskE(){
	struct resource mA = {&mutexA, 76}; 
	struct resource mB = {&mutexB, 76}; 

	struct task_t task_low_t = {25, mA.mutex_prio};
	struct task_t task_high_t = {50, mA.mutex_prio};

	RT_TASK task_low;
 	RT_TASK task_high;
	RT_TASK task_sync;
	
	//creation of the mutex
	rt_sem_create(&sem, "semaphore", 0, S_FIFO);
	rt_mutex_create(&mutexA, "mutexA");
	rt_mutex_create(&mutexB, "mutexB");
    rt_printf("semaphore and mutex created \n");


    rt_task_create(&task_low, "task low", 0, task_low_t.base_prio, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_high, "task high", 0, task_high_t.base_prio, T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_sync, "task sync", 0, 80, T_CPU(1) | T_JOINABLE);
 	rt_printf("tasks created \n\n");

	rt_task_start(&task_sync, function_sync, NULL);
    rt_task_start(&task_low, func_low_taskE, NULL);
    rt_task_start(&task_high, func_high_taskE, NULL);
	rt_printf("tasks started \n");

	rt_task_join(&task_low);
	rt_task_join(&task_high);
	rt_task_join(&task_sync);
    rt_printf("end \n");	
}

void func_low_taskE(){
    set_cpu(1);	
	rt_printf("L waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);

	iccp_lock(&mA, &task_low_t);
	rt_printf("L Locked mutexA \n");

	rt_printf("L busy wait... 3 time units (A) \n");
	busy_wait_us(3 * PERIOD);
	rt_printf("L completed (A) \n");

	iccp_lock(&mB, &task_low_t);
	rt_printf("L Locked mutexB \n");

	rt_printf("L busy wait... 3 time units (B) \n");
	busy_wait_us(3 * PERIOD);
	rt_printf("L completed (B) \n");

	rt_printf("L mutexB unlocked \n");
	icpp_unlock(&mB, &task_low_t);

	rt_printf("L mutexA unlocked \n");
	icpp_unlock(&mA, &task_low_t);

	rt_printf("L busy wait... 1 time unit \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("L completed \n");
}

void func_high_taskE(){
    set_cpu(1);
	rt_printf("H waiting for sync\n");
	rt_sem_p(&sem, TM_INFINITE);

	rt_printf("H sleep 1 time unit (B) \n");
	rt_task_sleep(1 * PERIOD);

	icpp_lock(&mB, &task_high_t);
	rt_printf("H Locked mutexB \n");

	rt_printf("H busy wait... 1 time unit (B) \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("H completed (B) \n");

	icpp_lock(&mA, &task_high_t);
	rt_printf("H Locked mutexA \n");

	rt_printf("H busy wait... 2 time units (A) \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("H completed (A) \n");

	rt_printf("H mutexA unlocked \n");
	icpp_unlock(&mA, &task_high_t);

	rt_printf("H mutexB unlocked \n");
	icpp_unlock(&mB, &task_high_t);

	rt_printf("H busy wait... 1 time unit \n");
	busy_wait_us(1 * PERIOD);
	rt_printf("H completed \n");
}


int main(){
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_task_shadow(NULL, "main task", 50, T_CPU(1));
   	// Call these at the start of mainf
	
    // do periodic work ...
    //TaskA();
	//TaskB();
	//TaskC();
	//TaskD();
	TaskE();

	return 0;
}
