#include "util.h"

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

void resolution_clock(){
    int ns_max = 500;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    struct timespec t1;
    struct timespec t2;

    for(int i = 0; i < 10*1000*1000; i++){
        clock_gettime(CLOCK_MONOTONIC, &t1);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &t2);

        struct timespec tick_normalized = timespec_sub(t2, t1);
        uint64_t tick_ns = tick_normalized.tv_sec*pow(10, 9) + tick_normalized.tv_nsec;

        if(tick_ns >= 0 && tick_ns < (uint64_t)ns_max){
            histogram[tick_ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}
