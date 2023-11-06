//Basic
#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;
char buffer[100];
FILE *file;
#define MAX_CHARS 10 // define a constant for the maximum number of characters

void * writer(void *arg) {
  
  //char buffer[100];
  pthread_mutex_lock(&read_mutex);
  pthread_mutex_lock(&write_mutex);
  sleep(5);
  printf("\n File locked by writer, please write on the file... \n");
  file=fopen("test.txt","w");
  fgets(buffer, sizeof(buffer), stdin);
  if (strlen(buffer) > MAX_CHARS) { // check if the input is longer than the maximum
    printf("\n Warning: input is longer than %d characters. \n", MAX_CHARS);
    buffer[MAX_CHARS] = '\0'; // truncate the input to the maximum
  }
  fprintf(file,"%s", buffer);
  fclose(file);
  pthread_mutex_unlock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);
  
  printf("\n File unlocked by writer. \n");
  return 0;
}

void * reader(void *arg) {
  //FILE *file;
  
  int i; // declare a variable to store the loop index
  pthread_mutex_lock(&read_mutex);
  pthread_mutex_lock(&write_mutex);
  sleep(5);
  
  printf("\nReader is opening file... \n");
  file=fopen("test.txt","r");
  while (fgets(buffer, sizeof(buffer), file)){
    printf("\n File content is: %s \n\n", buffer); // print the file content
    for (i = 0; i < strlen(buffer); i++) { // loop over each character in the buffer
      printf("[%c:%d] ", buffer[i], i); // print the character and its index in the buffer
    }
    printf("\n"); // print a new line
  }
  fclose(file);
  pthread_mutex_unlock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);
  
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
