#include <iostream>

using namespace std;

int main() {
    int num1 = 100;
    int &numRef = num1;
    int *numPtr = &num1;
    cout << num1 << endl;
    cout << numRef << endl;
    cout << numPtr << endl;
    return 0;

}