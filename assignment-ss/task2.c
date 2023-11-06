#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
// repeat 100 times to mimic 100 random visits to the page
#define RPT 100
// web page visit counter
int cnt = 0;
pthread_mutex_t mutex_counter;
void *counter()
{
   pthread_mutex_lock(&mutex_counter);
    int cntLocalCopy;
    float r;
    cntLocalCopy = cnt;
    // mimicking the work of the sever in serving the page to
    // the browser
    r = rand() % 2000;
    usleep(r);
    cnt = cntLocalCopy + 1;
    pthread_mutex_unlock(&mutex_counter);
    return NULL;
}
int main()
{
    int i;
    float r;
    pthread_t tid[RPT];
    // seed the random number sequence
    pthread_mutex_init(&mutex_counter,NULL);
    
    srand(time(NULL));
    for (i = 0; i < RPT; i++)
    {
        // mimicking the random access to the web page
        r = rand() % 2000;
        usleep(r);
        // a thread to respond to a connection request
        pthread_create(&tid[i], NULL, &counter, NULL);
    }
    // Wait till threads complete.
    for (i = 0; i < RPT; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex_counter);
    // print out the counter value and the number of mimicked visits
    // the 2 values should be the same if the program is written
    // properly
    printf("cnt=%d, repeat=%d\n", cnt, RPT);
}