#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t forks[5];

void take_fork(int id){
        if(id == 0) {
            pthread_mutex_lock(&forks[id]);
            printf("phil %i \t Picked up right fork %i...\n", id, id);
            pthread_mutex_lock(&forks[(id-1+5)%5]);
            printf("phil %i \t Picked up left fork %i...\n", id, (id-1+5)%5);
        }
        else {
            pthread_mutex_lock(&forks[(id-1+5)%5]);
            printf("phil %i \t Picked up left fork %i...\n", id, (id-1+5)%5);
            pthread_mutex_lock(&forks[id]);
            printf("phil %i \t Picked up right fork %i...\n", id, id);
        }
}

void put_fork(int id){
        pthread_mutex_unlock(&forks[id]);
        printf("phil %i \t Put down right fork %i...\n", id, id);
        pthread_mutex_unlock(&forks[(id-1+5)%5]);
        printf("phil %i \t Put down left fork %i...\n", id, (id-1+5)%5);
}

void* eat(void* num){
    int id = *(int*)num;
    while(1) {
        take_fork(id); 
        sleep(1); 
        printf("phil %i \t eating...\n", id);
        put_fork(id); 
        printf("phil %i \t Thinking...\n\n", id);
        sleep(1); 
    }
    return NULL;
}

int main(){
    pthread_t phil[5];
    int i, id[5];

    for (i = 0; i < 5; i++)
		pthread_mutex_init(forks + (id[i] = i), NULL);

	for (i = 0; i < 5; i++)
		pthread_create(phil + i, NULL, eat, id + i);

    return pthread_join(phil[0], NULL);
}