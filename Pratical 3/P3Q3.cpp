/*
	P3Q3: ask for 4 digit integer then display as thousands, hundreds, tens and ones
*/

#include <iostream>
using namespace std;

int main()
{
	// declare to store user input
	int number = 0;

	cout << "Enter a 4-digit integer : ";
	// get user input
	cin >> number;

	// check for thousands
	int thousand = (number / 1000) * 1000;
	// check for hundreds
	int hundred = (number / 100) % 10 * 100;
	// check for tens
	int ten = (number / 10) % 10 * 10;
	// check for ones
	int ones = number % 10;

	// display result
	cout << thousand << " + " << hundred << " + " << ten << " + " << ones << endl;
	return 0;
}
