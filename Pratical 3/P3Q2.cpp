#include <iostream>

using namespace std;

int main(){

    int num, d1, d2, d3, d4;
    cout << "Enter a four-digit number: ";
    cin >> num ;

    d1 = num / 1000;
    d2 = (num / 100) % 10;
    d3 = (num / 10) % 10;
    d4 = num % 10;

    cout << "\n1st digit :" << d1 << endl;
    cout << "2nd digit :" << d2 << endl;
    cout << "3rd digit :" << d3 << endl;
    cout << "4th digit :" << d4 << endl;

    return 0;

}
