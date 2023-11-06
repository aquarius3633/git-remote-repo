#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define NUM_OF_THREADS 2
#define BUFFER_SIZE 10

pthread_mutex_t buffer_mutex;
sem_t *bufferSpaceEmpty;
sem_t *bufferSpaceFull;
int buffer[BUFFER_SIZE]; //shared buffer size
int bufferCount = 0; //track number of elements in buffer
int nextIn = 0;// write
int nextOut = 0;//read
//producer thread
void* writer(void* arg){
        char c;
        //while(1){
            sem_wait(bufferSpaceEmpty);
            pthread_mutex_lock(&buffer_mutex);
            sleep(2);
            printf("writer thread locked\n");
            
            //while(bufferCount < BUFFER_SIZE){
            while((c = getchar()) != '\n' || bufferCount < BUFFER_SIZE) {  
                    buffer[nextIn] = c;
                    //printf("%c\n", buffer[nextIn]);
                    nextIn = (nextIn + 1) % BUFFER_SIZE;
                    bufferCount++;
                    //printf("%d\n", bufferCount);  
            }

            pthread_mutex_unlock(&buffer_mutex);
            sem_post(bufferSpaceFull);
            printf("writer thread unlocked\n");
        //}
    return NULL;
}

//consumer thread
void* reader(void* arg){
        char c;
        //while(1){
            sem_wait(bufferSpaceFull);
            pthread_mutex_lock(&buffer_mutex);
            printf("Reader thread locked\n");
            while(bufferCount > 0){
                c = buffer[nextOut];
                printf("[%c : %d]\n", c, bufferCount - 1);
                printf("the character removed: %c\n", c);
                bufferCount--;
                buffer[nextOut] = '\0';
                nextOut = (nextOut + 1) % BUFFER_SIZE;
                printf("test nextOut: %c\n", buffer[nextOut]);
            } 
            //if buffersize is empty print out buffer is empty
            printf("the buffer is empty\n");
    
            sem_post(bufferSpaceEmpty);
            pthread_mutex_unlock(&buffer_mutex);
            printf("Reader thread unlocked\n");
        //}
        
        return NULL;
}

int main(int argc, char* argv[]){
        pthread_t th[NUM_OF_THREADS]; 
        pthread_mutex_init(&buffer_mutex,NULL);
        //sem_init(&bufferSpaceEmpty,0, BUFFER_SIZE);
        //sem_init(&bufferSpaceFull,0, 0);
        //buffer space empty 10
        //buffer space full 0
        bufferSpaceEmpty = sem_open("/empty_sem", O_CREAT, 0644, BUFFER_SIZE);
        if (bufferSpaceEmpty == SEM_FAILED) {
            perror("Failed to open semphore for empty");
            exit(-1);
        }

        bufferSpaceFull = sem_open("/full_sem", O_CREAT, 0644, 0);
        if (bufferSpaceFull == SEM_FAILED) {
            sem_close(bufferSpaceEmpty);
            perror("Failed to open semphore for full");
            exit(-1);
        }


        for(int i = 0; i < NUM_OF_THREADS; i++){
            if(i % 2 == 0){
                if(pthread_create(&th[i], NULL, &writer, NULL)){
                    perror("pthread_create failed\n");
                };
            }else{
                if(pthread_create(&th[i], NULL, &reader, NULL)){
                    perror("pthread_create failed\n");
                }
            }
            
        }
        for(int i = 0; i < NUM_OF_THREADS; i++){
            if(pthread_join(th[i], NULL)){
                perror("pthread_join failed\n");
            }
        }
        
        sem_close(bufferSpaceEmpty);
        sem_close(bufferSpaceFull);
        pthread_mutex_destroy(&buffer_mutex);
    return 0;
}