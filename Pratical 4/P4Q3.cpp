/*
	P4Q3: Get 3 integers from user and print the sum, product, average, smallest, largest
	with pseudocode
	
*/

#include <iostream>
#include <iomanip>
using namespace std;

int main3()
{
	// declare variables
	int num1, num2, num3, sum, product, smallest, largest;
	double average;

	// prompt and get inputs
	cout << "Enter three integers : " << endl;
	cin >> num1 >> num2 >> num3;

	// calculate sum
	sum = num1 + num2 + num3;
	// calculate product
	product = num1 * num2 * num3;
	// calculate average
	average = (double)sum / 3;

	// if statement to check smallest
	smallest = num1;
	if (num2 < smallest)
		smallest = num2;
	if (num3 < smallest)
		smallest = num3;

	// if statement to check largest
	largest = num1;
	if (num2 > largest)
		largest = num2;
	if (num3 > largest)
		largest = num3;

	// display result
	cout << endl;
	cout << "The sum is : " << sum << endl;
	cout << "The product is : " << product << endl;
	cout << "The average is : " << fixed << setprecision(2) << average
		<< endl;
	cout << "The smallest value is : " << smallest << endl;
	cout << "The largest value is : " << largest << endl;
	cout << endl;
	return 0;
}

/*
Pseudocode
START
	Variable definition and initialization
	Read num1, num2, num3
	Compute sum = num1 + num2 + num3
	Compute product = num1 * num2 * num3
	Compute average = sum / 3
	Set smallest = num1
	IF (num2 < smallest)
		Set smallest = num2
	ENDIF
	IF (num3 < smallest)
		Set smallest = num3
	ENDIF
	Set largest = num1
	IF (num2 > largest)
		Set largest = num2
	ENDIF
	IF (num3 > largest)
		Set largest = num3
	ENDIF
	Print sum, product, average, smallest, largest
END

*/