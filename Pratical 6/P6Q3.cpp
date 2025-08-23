#include <iostream>
#include <cmath> // import for math operation
using namespace std;
#define PI 3.14159

int main() {
	// declare variables
	double a, b, c, deg, rad;

	// get input
	cout << "Enter length of side b: ";
	cin >> b;
	cout << "Enter length of side c: ";
	cin >> c;
	cout << "Enter angle (degree) : ";
	cin >> deg;

	// Convert degree to radian
	rad = deg * PI / 180.0;

	// ?? use sqrt, pow, cos to calculate the length of side
	a = sqrt(pow(b, 2) + pow(c, 2) - (2 * b * c * cos(rad)));

	cout << "\nThe length of side a is " << a;
	return 0;
}
