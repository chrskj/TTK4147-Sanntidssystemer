#include <pthread.h>

long g = 0;

// Note the argument and return types: void*
void* fn(void* args){
    long loc = 0;
    for(int i=0; i<50000000; i++){
        g++;
        loc++;
    }
    return NULL;
}

int main(){
    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, fn, NULL);  
    pthread_create(&threadHandle, NULL, fn, NULL);    
    pthread_join(threadHandle, NULL);
}