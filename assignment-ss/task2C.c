#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#define BUFFER_SIZE 10
sem_t bufferFull, bufferEmpty;
pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;

void *writer(void *arg)
{
    FILE *file;
    char buffer[BUFFER_SIZE];
    sem_wait(&bufferEmpty);
    pthread_mutex_lock(&write_mutex);
    sleep(2);
    pthread_mutex_lock(&read_mutex);
    printf("\n File locked by writer, please write on the file... \n");
    file = fopen("test.txt", "w");
    int i;
    while (fgets(buffer, sizeof(buffer), file)){
            printf("\n File content is: %s \n\n", buffer); // print the file content
            for (i = 0; i < strlen(buffer); i++) { // loop over each character in the buffer
            printf("[%c:%d] ", buffer[i], i); // print the character and its index in the buffer
        }
        printf("\n"); // print a new line
    }
    
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
    char buffer[10];
    int count = 0;
    sem_wait(&bufferFull);
    pthread_mutex_lock(&write_mutex);
    //sleep(5);
    pthread_mutex_lock(&read_mutex);
    printf("\nReader is opening file... \n");
    file = fopen("test.txt", "r");
    
    while (fgets(buffer, sizeof(buffer), file))
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            count++;
            printf("[%c:%d]", buffer[i], 10 - count);
        }
    }
    
    fclose(file);
    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);
    sem_post(&bufferEmpty);
    return 0;
}
int main(void)
{
    sem_init(&bufferEmpty, 0, BUFFER_SIZE);
    sem_init(&bufferFull, 0, 0);
    pthread_mutex_init(&write_mutex, NULL);
    pthread_mutex_init(&read_mutex, NULL);
    pthread_t thread_id_write, thread_id_read;
    if (pthread_create(&thread_id_write, NULL, &writer, NULL))
    {
        printf("error creating thread.");
        abort();
    }
    sleep(1);
    if (pthread_create(&thread_id_read, NULL, &reader, NULL))
    {
        printf("error creating thread.");
        abort();
    }
    else
        printf("\nThreads successflly created.");
    if (pthread_join(thread_id_write, NULL))
    {
        printf("error joining thread.");
        abort();
    }
    if (pthread_join(thread_id_read, NULL))
    {
        printf("error joining thread.");
        abort();
    }
    sem_destroy(&bufferEmpty);
    sem_destroy(&bufferFull);
    pthread_mutex_destroy(&read_mutex);
    pthread_mutex_destroy(&write_mutex);
}