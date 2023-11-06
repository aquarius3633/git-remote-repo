#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define NUM_OF_THREADS 2
#define BUFFER_SIZE 10

pthread_mutex_t buffer_mutex;
int buffer[BUFFER_SIZE]; //shared buffer size
int count = 0; //track number of elements in buffer
sem_t bufferSpaceEmpty;
sem_t bufferSpaceFull;

int nextIn = 0;
int nextOut = 0;
//producer thread
void* prod(void* arg){
    pthread_mutex_lock(&buffer_mutex);
    sem_wait(&bufferSpaceEmpty);
    printf("writer thread locked\n");
    char c = getchar();
    buffer[nextIn] = c;
    nextIn = (nextIn + 1) % BUFFER_SIZE;
    sem_post(&bufferSpaceFull);
    pthread_mutex_unlock(&buffer_mutex);
    printf("writer thread unlocked\n");
}

//consumer thread
void* consumer(void* arg){
    pthread_mutex_lock(&buffer_mutex);
    sem_wait(&bufferSpaceEmpty);
    printf("Reader thread locked\n");
    char c = buffer[nextOut];
    nextOut = (nextOut + 1) % BUFFER_SIZE;
    sem_post(&bufferSpaceFull);
    pthread_mutex_unlock(&buffer_mutex);
    printf("[%c : %d]\n", c, BUFFER_SIZE - nextOut);
    printf("Reader thread unlocked\n");
}

int main(int argc, int** argv){

    pthread_t th[NUM_OF_THREADS];
    pthread_mutex_init(&buffer_mutex,NULL);
    sem_init(&bufferSpaceEmpty,0, 10);
    sem_init(&bufferSpaceFull,0, 0);

    for(int i = 0; i < NUM_OF_THREADS; i++){
        if(i % 2 == 0){
            if(pthread_create(&th[i], NULL, &prod, NULL)){
                perror("pthread_create failed\n");
            };
        }else{
            if(pthread_create(&th[i], NULL, &consumer, NULL)){
                perror("pthread_create failed\n");
            }
        }
        
    }
    for(int i = 0; i < NUM_OF_THREADS; i++){
        if(pthread_join(th[i], NULL)){
            perror("pthread_join failed\n");
        }
    }
    
    sem_destroy(&bufferSpaceEmpty);
    sem_destroy(&bufferSpaceFull);
    pthread_mutex_destroy(&buffer_mutex);
    return 0;
}