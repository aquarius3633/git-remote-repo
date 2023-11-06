#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
// repeat the times (*) operation 100,000 times to slow down the
// execution of the instructions
#define RPT 100000
void* print_X() {
int i;
float x;
while (1) {
// the multiplication operation below is for consuming some CPU
//time to slow down the execution
for(i=0; i<RPT; i++) { x = 3.1415926*3.1415926; }
printf("X");
}
}
void* print_O() {
int i;
float x;
while (1) {
// the multiplication operation below is for consuming some CPU
//time to slow down the execution
for(i=0; i<RPT; i++) { x = 3.1415926*3.1415926; }
printf("O");
}
}
int main () {
pthread_t tid1, tid2;
pthread_create (&tid1, NULL, &print_X, NULL);
pthread_create (&tid2, NULL, &print_O, NULL);
// Wait till threads complete. Not really related to this
// question in this assignment. Can be ignored.
pthread_join(tid1, NULL);
pthread_join(tid2, NULL);
printf ("\n==========================\n");
}
