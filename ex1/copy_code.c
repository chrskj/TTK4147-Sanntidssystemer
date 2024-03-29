#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

struct timespec timespec_normalized(time_t sec, long nsec)
{
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

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs)
{
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs)
{
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int timespec_cmp(struct timespec lhs, struct timespec rhs)
{
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}

void busy_wait(struct timespec t)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec then = timespec_add(now, t);
    
    while(timespec_cmp(now, then) < 0){
        for(int i = 0; i < 10000; i++){}
        clock_gettime(CLOCK_MONOTONIC, &now);
    }
}

for(int i = 0; i < 10*1000*1000; i++)
{
    // read timer
}

int ns_max = 50;
int histogram[ns_max];
memset(histogram, 0, sizeof(int)*ns_max);

for(int i = 0; i < 10*1000*1000; i++)
{
    // t1 = timer()
    // t2 = timer()
    
    int ns = // (t2 - t1) * ??
    
    if(ns >= 0 && ns < ns_max){
        histogram[ns]++;
    }
}

for(int i = 0; i < ns_max; i++)
{
    printf("%d\n", histogram[i]);
}

long xy_size    = 1000*1000*1000;       // 8 GB (sizeof(long) = 8 bytes)
long x_dim      = 100;
long y_dim      = xy_size/x_dim;   
 
long** matrix   = malloc(y_dim*sizeof(long*));

for(long y = 0; y < y_dim; y++)
{
    matrix[y] = malloc(x_dim*sizeof(long));
}

printf("Allocation complete (press any key to continue...)\n");
getchar();
