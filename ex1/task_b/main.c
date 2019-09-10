#include "util.h"

#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc == 2){
        switch (*argv[1]){
            case 'a':
                printf("This is a");
                break;
            case 'b':
                printf("This is b");
                break;
            case 'c':
                printf("This is c\n");
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
