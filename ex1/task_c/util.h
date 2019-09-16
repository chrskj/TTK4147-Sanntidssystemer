#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>

#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <x86intrin.h>
#include <sched.h>

void busy_wait(struct timespec t);
void busy_wait_times(struct tms t);
void latency_rdtsc(int n);
void resolution_rdtsc();
void latency_clock(int n);
void resolution_clock();
void latency_times(int n);
void resolution_times();
