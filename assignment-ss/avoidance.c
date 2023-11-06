#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;

void * writer(void *arg) {
  FILE *file;
  char buffer[100];
  pthread_mutex_lock(&write_mutex);
  sleep(5);
  pthread_mutex_lock(&read_mutex);
  printf("\n File locked by writer, please write on the file... \n");
  file=fopen("test.txt","w");
  fgets(buffer, sizeof(buffer), stdin);
  fprintf(file,"%s", buffer);
  fclose(file);
  pthread_mutex_unlock(&read_mutex);
  pthread_mutex_unlock(&write_mutex);
  printf("\n File unlocked by writer. \n");
  return 0;
}

void * reader(void *arg) {
  FILE *file;
  char buffer[100];
  pthread_mutex_lock(&write_mutex);
  sleep(5);
  pthread_mutex_lock(&read_mutex);
  printf("\nReader is opening file... \n");
  file=fopen("test.txt","r");
  while (fgets(buffer, sizeof(buffer), file)){
    printf("\n File content is: %s \n\n", buffer);
  }
  fclose(file);
  pthread_mutex_unlock(&read_mutex);
  pthread_mutex_unlock(&write_mutex);
  return 0;
}

int main(void) {

  pthread_t thread_id_write,thread_id_read;
  if (pthread_create(&thread_id_write,NULL,&writer,NULL)){
    printf("error creating thread.");
    abort();
  }
  sleep(1);
  if (pthread_create(&thread_id_read,NULL,&reader,NULL)){
    printf("error creating thread.");
    abort();
  }
  else printf("\nThreads successflly created.");

  if (pthread_join(thread_id_write, NULL)){
    printf("error joining thread.");
    abort();
  }
  if (pthread_join(thread_id_read,NULL)){
    printf("error joining thread.");
    abort();
  }
}