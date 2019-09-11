#include "util.h"

struct timespec timespec_normalized(time_t sec, long nsec){
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

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int timespec_cmp(struct timespec lhs, struct timespec rhs){
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}

struct tms tms_add(struct tms lhs, struct tms rhs){
    return (struct tms){lhs.tms_utime + rhs.tms_utime,
        lhs.tms_stime + rhs.tms_stime,
        lhs.tms_cutime + rhs.tms_cutime,
        lhs.tms_cstime + rhs.tms_cstime};
}

 struct tms tms_sub(struct tms lhs, struct tms rhs){
     return (struct tms){lhs.tms_utime - rhs.tms_utime,
        lhs.tms_stime - rhs.tms_stime,
        lhs.tms_cutime - rhs.tms_cutime,
        lhs.tms_cstime - rhs.tms_cstime};
}

int tms_cmp(struct tms lhs, struct tms rhs){
    if (lhs.tms_stime < rhs.tms_stime)
        return -1;
    if (lhs.tms_stime > rhs.tms_stime)
        return 1;
    else return 0;
}

void busy_wait(struct timespec t){
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec then = timespec_add(now, t);
    while(timespec_cmp(now, then) < 0){
        for(int i = 0; i < 10000; i++){}
        clock_gettime(CLOCK_MONOTONIC, &now);
    }
}

void latency_rdtsc(int n){
    uint64_t start_tick = __rdtsc();
    uint64_t ticks;
    for(int i = 0; i < n; i++)
    {
        ticks = __rdtsc();
    }
    n = (uint64_t)n;
    ticks = (ticks - start_tick)/2.66;
    uint64_t latency = ticks / n;
    printf("ticks: %" PRIu64 "\n", ticks);
    printf("latency: %" PRIu64 "\n", latency);
}

void resolution_rdtsc(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    uint64_t t1;
    uint64_t t2;
    for(int i = 0; i < 10*1000*1000; i++){
        t1 = __rdtsc();
        t2 = __rdtsc();

        int ns = (t2 - t1) / 2.66;

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}

void latency_clock(int n){
    struct timespec ticks;
    struct timespec start_tick;
    clock_gettime(CLOCK_MONOTONIC, &start_tick);
    for(int i = 0; i < n; i++) {
        clock_gettime(CLOCK_MONOTONIC, &ticks);
    }
    struct timespec tick_normalized = timespec_sub(ticks, start_tick);
    uint64_t tick_ns = tick_normalized.tv_sec*pow(10, 9) + tick_normalized.tv_nsec;
    n = (uint64_t)n;
    uint64_t latency = tick_ns / n;
    printf("ticks: %" PRIu64 "\n", tick_ns);
    printf("latency: %" PRIu64 "\n", latency);
}

void resolution_clock(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    struct timespec t1;
    struct timespec t2;
    for(int i = 0; i < 10*1000*1000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t1);
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

void latency_times(int n){
    struct tms ticks;
    for(int i = 0; i < n; i++) {
        times(&ticks);
        //printf("ticks: %li \n", ticks.tms_stime);
    }
    int latency = (ticks.tms_stime / n)*(1000000000/100);
    printf("ticks: %li \n", ticks.tms_stime);
    printf("latency: %d \n", latency);
}

void resolution_times(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    struct tms t1;
    struct tms t2;
    for(int i = 0; i < 10*1000*1000; i++){
        times(&t1);
        times(&t2);
        //printf("%li, %li \n", t1.tms_stime, t2.tms_stime);

        int ns = (t2.tms_stime - t1.tms_stime)*(1000000000/100);

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
}
