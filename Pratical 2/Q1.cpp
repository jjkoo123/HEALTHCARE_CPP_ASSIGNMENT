/*
    Question ask you to get input first
    then perform data formatting

*/

// P2Q1, Output formatting
#include <iostream>
// ?? input/output manipulation, data formatting
#include <iomanip>
// ?? to include string library, optional
#include <string>

using namespace std;

int main()
{
    // define variables
    // ?? to store whole number without decimal points
    int num;
    // ?? to store number with decimal points
    double num2;
    // ?? to store value with space eg. name
    string name;
    // char, is to store single character

    // ?? prompt and get inputs
    cout << "Enter one interger: ";
    cin >> num;
    cout << "Enter one floating-point number: ";
    cin >> num2;

    // ?? is to solve next line problem, used to skip leftover character in memory
    cin.get();
    cout << "Enter user name: ";

    // cin >> inputThree; // cin, only can get value without space
    // ?? getline allow to get input with spaces/whole string until new line
    getline(cin, name);

    // ?? Q1 part a
    // left -> display output from left hand side
    // setw -> set width, is to create spacing for output
    // dec -> change value to decimal format (base 10)
    // hex -> change value to hexadecimal format (base 16)
    // oct -> change value to octal format (base 8)
    cout << "\nQ1 Part A" << endl;
    cout << "Decimal Format: " << left << setw(6) << dec << num << endl; 
    cout << "Hexadecimal Format: " << left << setw(6) << hex << num << endl;
    cout << "Octal Format: " << left << setw(6) << oct << num << endl;

    // ?? Q1 Part b
    // fixed = make sure your value display with decimal point
    // setprecision(4) = is to have 4 decimal point
    // showpos = is to show +/- sign
    cout << "\nQ1 Part B" << endl;
    cout << "Fixid notation: " << fixed << setprecision(4) << showpos << num2 << endl;

    // scientific = is to show scientific symbol eg. xxxe+2
    cout << "Scientific notation: " << fixed << setprecision(4) << scientific << num2 << endl;

    // ?? Q1 Part C
    // right, move to right hand side
    // setfill(*), unused space fill-in with *
    cout << "\nQ1 Part C" << endl;
    cout << "String value: " << right << setw(25) << setfill('*') << name << endl;

    return 0;
}