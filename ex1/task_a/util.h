#pragma once
#include <time.h>
#include <sys/times.h>
#include <stdio.h>

void busy_wait(struct timespec t);
void busy_wait_times(clock_t interval);

