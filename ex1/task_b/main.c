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
                break;
            case 'b':
                resolution_rdtsc();
                break;
            case 'c':
                latency_clock(10*1000*1000);
                break;
            case 'd':
                resolution_clock();
                break;
            case 'e':
                latency_times(10*1000*1000);
                break;
            case 'f':
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
