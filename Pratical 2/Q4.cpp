#include <iostream>
#include <cmath> // for floor()

using namespace std;

int main()
{
    double number;
    cout << "Enter a floating-point number > ";
    cin >> number;

    // Extract integral part
    int integralPart = static_cast<int>(number);

    // Extract fractional part
    double fractionalPart = number - integralPart;

    // If the number is negative, adjust fractional part to be positive
    if (fractionalPart < 0)
    {
        fractionalPart *= -1;
    }

    // Display results
    cout << "\nIts integral part   : " << integralPart << endl;
    cout << "Its fractional part : " << fractionalPart << endl;

    return 0;
}
