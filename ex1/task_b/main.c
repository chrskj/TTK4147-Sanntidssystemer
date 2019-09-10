#include "util.h"

#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>
#include <inttypes.h>

int main(int argc, char *argv[]){
    if(argc == 2){
        switch (*argv[1]){
            case 'a':
                latency_rdtsc(10*1000*1000);
                resolution_rdtsc();
                break;
            case 'b':
                latency_clock(10*1000*1000);
                resolution_clock();
                break;
            case 'c':
                latency_times(10*1000*1000);
                resolution_times();
                break;
        }
    }
    else if(argc > 2){
        printf("Too many arguments supplied.\n");
    }
    else{
        printf("One argument expected.\n");
    }
}
