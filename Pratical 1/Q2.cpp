#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main()
{
    string name;
    int age;
    double income;
    char gender;

    cout << "Enter your <Name> <Age> <Gender> <Income> " << endl;
    cin >> name >> age >> gender >> income;

    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Gender: " << gender << endl;
    cout << "Income: " << fixed << setprecision(2) << showpoint << income << endl;

    return 0;
}