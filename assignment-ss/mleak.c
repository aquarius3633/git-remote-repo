#include <stdio.h> 
#include <stdlib.h> 

void mleak_routine(){
    //int* p = (int*) malloc(sizeof(int) *  1048576 ); //1048576 is 1MB
    int* p = (int*) malloc(sizeof(int) * 1024); // 1024 is 4KB

}

int main ()  
{ 
    long long count = 0;
    while(1){
        mleak_routine(); 
        //count += 1048576;
        count += sizeof(int) * 1024;
        //printf("Memory leaked so far: %d bytes\n", count);
        double gb = (double)count / (1024 * 1024 * 1024);
        printf("Memory leaked so far: %.2f GB\n", gb);
    }
    return 0; 
} 