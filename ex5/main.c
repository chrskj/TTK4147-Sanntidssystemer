#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"

#define TEST_A 1
#define TEST_B 2
#define TEST_C 3

/// Assigning CPU core ///
int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

/// 'struct timespec' functions ///
struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

struct responseTaskArgs {
	struct {
		uint32_t channel;
	} test;
	// other args ...
};

// Note the argument and return types: void*
void* taskABC(void* args){
    set_cpu(1);
    struct responseTaskArgs a = *(struct responseTaskArgs*)args;
    //printf("%d \n", io_read(a.test.channel));
    while(1){
        if(io_read(a.test.channel) == 0){
            io_write(a.test.channel, 0);
            //printf("this should be 0: %d\n", io_read(a.test.channel));
            //printf("A should be 0 %u\n", gpio_get_pin_value(RESPONSE_A));
            usleep(10);
            io_write(a.test.channel, 1);
            //printf("this should be 1: %d\n", io_read(a.test.channel));
        }
    }    
}

void* taskD(void* args){
    set_cpu(1);

    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    struct timespec period = {.tv_sec = 0, .tv_nsec = 500*1000};

    struct responseTaskArgs a = *(struct responseTaskArgs*)args;

    while(1){
        if(io_read(a.test.channel) == 0){
            // Do periodic work
            io_write(a.test.channel, 0);
            usleep(10);
            io_write(a.test.channel, 1);
        }
        // Sleep    
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }    
}

void* disturb(void* args){
    set_cpu(1);
    while(1){
        asm volatile("" ::: "memory");
    }  
}

int main(){
    io_init();

    pthread_t distList[1];
    for (int i = 0; i < 10; i++){
        pthread_create(&distList[i], NULL, disturb, NULL);
    }

    // do periodic work ...
    pthread_t threadTestA;
    pthread_t threadTestB;
    pthread_t threadTestC;
    /* Task ABC
    pthread_create(&threadTestA, NULL, taskABC, (&(struct responseTaskArgs){{TEST_A}}));
    pthread_create(&threadTestB, NULL, taskABC, (&(struct responseTaskArgs){{TEST_B}})); 
    pthread_create(&threadTestC, NULL, taskABC, (&(struct responseTaskArgs){{TEST_C}})); 
    */
    pthread_create(&threadTestA, NULL, taskD, (&(struct responseTaskArgs){{TEST_A}}));
    pthread_create(&threadTestB, NULL, taskD, (&(struct responseTaskArgs){{TEST_B}})); 
    pthread_create(&threadTestC, NULL, taskD, (&(struct responseTaskArgs){{TEST_C}})); 
    pthread_join(threadTestA, NULL);
    pthread_join(threadTestB, NULL);
    pthread_join(threadTestC, NULL);

    for (int i = 0; i < 10; i++){
        pthread_join(distList[i], NULL);
    }

    return 0;
}