#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define BUFFER_SIZE 10

char buffer[BUFFER_SIZE];
int buffer_count = 0;
int buffer_read_index = 0;
int buffer_write_index = 0;
pthread_mutex_t mutex;
sem_t bufferSpaceEmpty, bufferSpaceFull;

void* producer(void* arg) {
    char c;
    
    while (1) {
        c = getchar();
        if (isspace(c)) {
            continue; // skip whitespace characters
        }
        sem_wait(&bufferSpaceEmpty);  // wait for space in the buffer
        pthread_mutex_lock(&mutex);
        buffer[buffer_write_index] = c;
        printf("Producer added '%c' in buffer, %d characters in buffer, %d remaining space available\n", c, buffer_count + 1, BUFFER_SIZE - buffer_count - 1);
        buffer_write_index = (buffer_write_index + 1) % BUFFER_SIZE;
        buffer_count++;
        if(buffer_count == BUFFER_SIZE){
            printf("-------buffer is current full--------\n");
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&bufferSpaceFull);  // signal that a new character is available
    }
    return NULL;
}

void* consumer(void* arg) {
    
    while (1) {
        sem_wait(&bufferSpaceFull);  // wait for a new character to be available
        pthread_mutex_lock(&mutex);
        char c = buffer[buffer_read_index];
        printf("Consumer removed '%c' from buffer, %d remaining characters in buffer, %d remaining spaces in buffer\n", c, buffer_count-1, BUFFER_SIZE - buffer_count + 1);
        buffer[buffer_read_index] = '\0'; //remove character from the buffer
        buffer_read_index = (buffer_read_index + 1) % BUFFER_SIZE;
        buffer_count--;
        if(buffer_count == 0){
            printf("--------buffer is current empty--------\n");
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&bufferSpaceEmpty);  // signal that there is space in the buffer
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&bufferSpaceEmpty, 0, BUFFER_SIZE);
    sem_init(&bufferSpaceFull, 0, 0);
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&bufferSpaceEmpty);
    sem_destroy(&bufferSpaceFull);
    return 0;
}