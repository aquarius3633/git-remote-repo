#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

void mleakRoutine(){
    int i;
    //dynamically allocate 100 integer space in heap
    int* arr = (int*) malloc(sizeof(int) * SIZE);
    //generate random number and store it in array
    for(i=0; i< SIZE; i++){
        arr[i] = rand() % SIZE;
    }
    //print out stored numbers from array
    for(i = 0; i < SIZE; i++){
        printf("%d ", arr[i]);
    }
    //deacllocate array memory sapce from heap
    //free(arr);
}

int main(int argc, char* argv[]){
    //seed for random number generation
    srand(time(NULL));
    //call function
    while(1){
        mleakRoutine();
    }
    
    return 0;
}