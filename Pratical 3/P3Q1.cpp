#include <iostream>

using namespace std;

int main(){

    int days;
    cout << "Enter the number of days: ";
    cin >> days;

    int weeks = days / 7; // Calculate weeks
    int rem_days = days % 7; // Calculate remaining days
    cout << days << " days = " << weeks << " weeks and " << rem_days << " days." << endl;

    return 0;

}