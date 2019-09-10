#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"

struct timespec duration = {1, 0};

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        switch (*argv[1])
        {
            case 'a':
                printf("This is a");
                sleep(1);
                break;
            case 'b':
                printf("This is b");
                busy_wait(duration);
                break;
            case 'c':
                printf("This is c");
                busy_wait_times(1);
                break;
        }
    }
    else if(argc > 2)
    {
        printf("Too many arguments supplied.\n");
    }
    else
    {
        printf("One argument expected.\n");
    }
}
