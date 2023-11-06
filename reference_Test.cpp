#include <iostream>
#define LOG(x) std::cout << x << std::endl;


//using namespace std;
void increment(int& value){
    value++;
}

int main() {
    int num1 = 100;
    int &numRef = num1;
    int* numPtr = &num1;
    //cout << "num1 = " << num1 << endl;
    // cout << "numRef = " << numRef << endl;
    // cout << "numPtr = " << numPtr << endl;
    LOG(num1);
    LOG(numRef);
    LOG(numPtr);

    increment(num1);//pass by reference

    // cout << "num1 = " << num1 << endl;
    LOG(num1);
    

    //pointer
    int* ptr = nullptr;

    ptr = &num1;

    LOG(ptr);
    LOG(*ptr);
    return 0;

}