/*
	P3Q5: get length and height from user and calculate area and triangle

*/

#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
	// ??: declare variable for height, length and area
	int height = 0, length = 0;
	double area = 0.0;

	// ??: prompt and get height and length
	cout << "Enter the height of the triangle in cm: " << endl;
	cin >> height;
	cout << "Enter the length of the triangle in cm: " << endl;
	cin >> length;

	// ??: calculate area
	area = 0.5 * height * length;

	// display result
	cout << "\n\nArea of triangle" << endl;
	cout << "---------------------------" << endl;
	cout << "Height : " << right << setw(10) << height << " cm" << endl;
	cout << "Length : " << right << setw(10) << length << " cm" << endl;
	cout << "Area   : " << right << setw(10) << area << " cm" << endl;
	return 0;
}
