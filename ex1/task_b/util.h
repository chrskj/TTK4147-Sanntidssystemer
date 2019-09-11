#pragma once
#include <time.h>
#include <sys/times.h>
#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void busy_wait(struct timespec t);
void latency_rdtsc(int n);
void resolution_rdtsc();
void latency_clock(int n);
void resolution_clock();
void latency_times(int n);
void resolution_times();
