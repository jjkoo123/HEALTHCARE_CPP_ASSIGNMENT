/*
	P3Q6: Ask for raduis, height, cost and amount from user
	then calculate the total cost
*/

#include <iostream>
#include <iomanip>
// ??: declare constant for PI
using namespace std;

int main()
{
	// ??: declare variables
	const double PI = 3.142; // constant for PI

	double r, h, cost, amount;
	// ??: prompt and get r, h, cost and amount
	cout << "Enter the radius of the cylinder in cm: " << endl;
	cin >> r;
	cout << "Enter the height of the cylinder in cm: " << endl;
	cin >> h;
	cout << "Please enter the cost of the material (in RM/cm2): " << endl;
	cin >> cost;
	cout << "Please enter the amount of cylinder : " << endl;
	cin >> amount;

	// ??: calculate the ttotal cost based on formula
	double result = (PI * r * r + 2 * PI * r * h) * cost * amount;

	// ??: display result
	cout << "The total cost of producing " << amount << " containers with " << r << "cm radius and "
		 << h << "cm height is RM" << fixed << setprecision(2) << result << endl;

	return 0;
}
