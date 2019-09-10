#include "util.h"

struct timespec timespec_normalized(time_t sec, long nsec)
{
    while(nsec >= 1000000000)
    {
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0)
    {
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

void busy_wait_times(clock_t interval)
{
    static struct tms st_cpu;
    static struct tms en_cpu;

    static clock_t st_time = times(&st_cpu);
    static clock_t en_time = st_time + interval;
    
    while(en_time - times(&en_cpu)){
        for(int i = 0; i < 10000; i++){}
    }
}
