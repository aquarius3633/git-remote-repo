#include <iostream>
#inlcude <vector>
#include <string>

int main() {

    vector v{4,8,1,9,3,6,2,7};

    sort(v.begin(), v.end());

    for(int i : v) {
        cout << " " << i << endl;
    }

    return 0;
}   