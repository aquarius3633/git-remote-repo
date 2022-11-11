#include <iostream>
using namespace std;  

void swap(int * a, int * b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSort(int a[], int size) {
    for(int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for(int j = i + 1; j < size; j++) {
            if(a[j] < a[minIndex]) {
                minIndex = j;
            }
        }
        swap(&a[minIndex], &a[i]);
    }
}

void insertionSort (int a[], int size) {
    for(int i = 1; i < size; i++) {
        int value = a[i];
        int index = i;
        while(value < a[index - 1] && index > 0) {
            a[index] = a[index - 1];
            index--;
        }
        a[index] = value;
    }
}

void quickSort (int a[], int size) {

}


void printArray(int a[], int size){
    for(int i = 0; i < size; i++){
        cout << a[i] << " ";
    }
}

int main() {
    int arr[] = {83, 9, 238, 23, 12, 48, 90, 100};
    int size = sizeof(arr)/sizeof(arr[0]);
    cout << "Before Sorting: " << endl;
    printArray(arr,size);
    //selectionSort(arr, size);
    insertionSort(arr, size);
    quickSort(arr, size);
    cout << endl;
    cout << "After Sorting: " << endl;
    printArray(arr, size);
}