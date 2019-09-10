#include "util.h"

#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc == 2){
        printf("Too many arguments supplied.\n");
    }
    else if(argc > 2){
        printf("Too many arguments supplied.\n");
    }
    else{
        printf("One argument expected.\n");
    }
}
