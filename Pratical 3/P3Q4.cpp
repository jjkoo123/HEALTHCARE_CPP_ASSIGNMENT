/*
	P3Q4: convert temperature in celsius and in fahrenheit based on formula
*/

#include <iostream>
using namespace std;

int main()
{
	// declare variable to store temparature in centigrade and fahrenheit for result
	double cen, fah;

	cout << "Enter the temperature in Celsius : ";
	// get temperature in centigrade
	cin >> cen;

	// convert as fahrenheit
	fah = (cen * 9.0 / 5.0) + 32.0;

	// display result
	cout << "The temperature in Fahrenheit is : " << fah << endl;
	return 0;

}
