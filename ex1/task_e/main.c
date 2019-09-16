#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

int main(){
    long capacity = 16;
    Array a = array_new(capacity);
    long data[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    for(int i = 0; i<capacity; i++){
        //printf("inserting %li \n", data[i]);
        array_insertBack(&a, data[i]);
    }

    array_print(a);
    return 0;
}
