#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"

struct timespec duration_b = {1, 0};
struct tms duration_c = {0, 100, 0, 0};

int main(int argc, char *argv[]){
    if(argc == 2){
        switch (*argv[1]){
            case 'a':
                printf("This is a");
                sleep(1);
                break;
            case 'b':
                printf("This is b");
                busy_wait(duration_b);
                break;
            case 'c':
                printf("This is c\n");
                busy_wait_times(duration_c);
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
