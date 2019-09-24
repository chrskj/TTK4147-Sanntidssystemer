#include <pthread.h>
#include <stdio.h>

long g = 0;

// Note the argument and return types: void*
void* fn(void* args){
    long loc = 0;
    for(int i=0; i<50000000; i++){
        g++;
        loc++;
    }
    printf("%ld, %ld\n", g, loc);
    return NULL;
}

int main(){
    pthread_t threadHandle1;
    pthread_t threadHandle2;
    pthread_create(&threadHandle1, NULL, fn, NULL);
    pthread_create(&threadHandle2, NULL, fn, NULL); 
    pthread_join(threadHandle1, NULL);
    pthread_join(threadHandle2, NULL);
}