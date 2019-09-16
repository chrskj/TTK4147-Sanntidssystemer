#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

int main(){
    long capacity = 2;
    Array a = array_new(capacity);
    long data[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};

    int len_data = sizeof(data)/sizeof(long);
    for(int i = 0; i<len_data; i++){
        printf("inserting %li %li %li %li \n", data[i], a.front, a.back, a.capacity);
        printf("inserting %p \n", &a);
        array_insertBack(&a, data[i]);
    }
    array_print(a);

    return 0;
}
