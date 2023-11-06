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

int buffer[BUFFER_SIZE]; // shared buffer size

int bufferCount = 0; // track number of elements in buffer

int nextIn = 0; // write

int nextOut = 0; // read

// producer thread

void *prod(void *arg)
{

    char c;

    sem_wait(bufferSpaceEmpty);

    pthread_mutex_lock(&buffer_mutex);

    sleep(2);

    printf("writer thread locked\n");

    while (bufferCount != BUFFER_SIZE)
    {

        c = getchar();

        if (c == '\n')
        {

            c = 0;

            break;
        }
        else
        {

            buffer[nextIn] = c;

            nextIn = (nextIn + 1) % BUFFER_SIZE;

            bufferCount++;
        }
    }

    pthread_mutex_unlock(&buffer_mutex);

    sem_post(bufferSpaceFull);

    printf("writer thread unlocked\n");

    return NULL;
}

// consumer thread

void *consumer(void *arg)
{

    char c;

    sem_wait(bufferSpaceFull);

    pthread_mutex_lock(&buffer_mutex);

    printf("Reader thread locked\n");

    while (bufferCount > 0)
    {

        c = buffer[nextOut];

        nextOut = (nextOut + 1) % BUFFER_SIZE;

        bufferCount--;

        printf("[%c : %d]\n", c, bufferCount);

        printf("the character removed: %c\n", buffer[nextOut - 1]);
    }

    pthread_mutex_unlock(&buffer_mutex);

    sem_post(bufferSpaceEmpty);

    printf("Reader thread unlocked\n");

    return NULL;
}

int main(int argc, char *argv[])
{

    while (1)
    {

        pthread_t th[NUM_OF_THREADS];

        pthread_mutex_init(&buffer_mutex, NULL);

        // sem_init(&bufferSpaceEmpty,0, BUFFER_SIZE);

        // sem_init(&bufferSpaceFull,0, 0);

        // buffer space empty 10

        // buffer space full 0

        bufferSpaceEmpty = sem_open("/empty_sem", O_CREAT, 0644, BUFFER_SIZE);

        if (bufferSpaceEmpty == SEM_FAILED)
        {

            perror("Failed to open semphore for empty");

            exit(-1);
        }

        bufferSpaceFull = sem_open("/full_sem", O_CREAT, 0644, 0);

        if (bufferSpaceFull == SEM_FAILED)
        {

            sem_close(bufferSpaceEmpty);

            perror("Failed to open semphore for full");

            exit(-1);
        }

        for (int i = 0; i < NUM_OF_THREADS; i++)
        {

            if (i % 2 == 0)
            {

                if (pthread_create(&th[i], NULL, &prod, NULL))
                {

                    perror("pthread_create failed\n");
                };
            }
            else
            {

                if (pthread_create(&th[i], NULL, &consumer, NULL))
                {

                    perror("pthread_create failed\n");
                }
            }
        }

        for (int i = 0; i < NUM_OF_THREADS; i++)
        {

            if (pthread_join(th[i], NULL))
            {

                perror("pthread_join failed\n");
            }
        }

        sem_close(bufferSpaceEmpty);

        sem_close(bufferSpaceFull);

        pthread_mutex_destroy(&buffer_mutex);
    }

    return 0;
}
