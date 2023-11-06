#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

sem_t bufferFull, bufferEmpty;
pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;
int buffer[BUFFER_SIZE];
int nextIn = 0, nextOut = 0, count = 0;

void *writer(void *arg)
{
    FILE *file;
    char c;
    sem_wait(&bufferEmpty);
    pthread_mutex_lock(&write_mutex);
    sleep(5);
    pthread_mutex_lock(&read_mutex);
    printf("\n File locked by writer, please write on the file... \n");
    file = fopen("test.txt", "w");
    // read one character at a time from the keyboard
    c = getchar();
    while (c != '\n')
    {
        // add the character to the buffer
        buffer[nextIn] = c;
        nextIn = (nextIn + 1) % BUFFER_SIZE;
        count++;
        sem_post(&bufferFull);
        sem_wait(&bufferEmpty);
        c = getchar();
    }
    // add end-of-line character to the buffer
    buffer[nextIn] = '\0';
    nextIn = (nextIn + 1) % BUFFER_SIZE;
    count++;
    sem_post(&bufferFull);
    sem_wait(&bufferEmpty);
    fprintf(file, "%s", buffer);
    fclose(file);
    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);
    sem_post(&bufferFull);
    printf("\n File unlocked by writer. \n");
    return 0;
}

void *reader(void *arg)
{
    FILE *file;
    char c;
    sem_wait(&bufferFull);
    pthread_mutex_lock(&write_mutex);
    sleep(5);
    pthread_mutex_lock(&read_mutex);
    printf("\nReader is opening file... \n");
    file = fopen("test.txt", "r");
    // read one character at a time from the buffer
    int i = nextOut;
    c = buffer[i];
    while (c != '\0')
    {
        // remove the characters from the buffer
        buffer[i] = ' ';
        nextOut = (nextOut + 1) % BUFFER_SIZE;
        count--;
