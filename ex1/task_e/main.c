#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

int main(){
    long capacity = 2;
    Array a = array_new(capacity);
    array_insertBack(&a, 1);
    array_print(a);
    array_insertBack(&a, 2);
    array_print(a);
    array_insertBack(&a, 3);
    array_print(a);
    array_insertBack(&a, 4);
    array_print(a);
    array_insertBack(&a, 5);
    array_print(a);
    array_insertBack(&a, 5);
    array_print(a);
    array_insertBack(&a, 7);
    array_print(a);
    array_insertBack(&a, 8);
    array_print(a);
    array_insertBack(&a, 9);
    array_print(a);
    array_destroy(a);
    return 0;
}
