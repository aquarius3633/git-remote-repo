void *reader(void *arg)
{
    FILE *file;
    char buffer[10];
    int count = 0;
    sem_wait(&bufferFull);
    pthread_mutex_lock(&write_mutex);
    sleep(5);
    pthread_mutex_lock(&read_mutex);
    printf("\nReader is opening file... \n");
    file = fopen("test.txt", "r");
    
    while (fgets(buffer, sizeof(buffer), file))
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            count++;
            printf("[%c:%d]", buffer[i], count);
        }
    }
    
    fclose(file);
    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);
    sem_post(&bufferEmpty);
    return 0;
}
