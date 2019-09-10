#pragma once
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <stdio.h>
#include <limits.h>

void busy_wait(struct timespec t);
void busy_wait_times(clock_t interval);

