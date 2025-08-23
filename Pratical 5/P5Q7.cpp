/*
	P5Q7: ask positive integer from user, stop when the value is negative or zero
	then sum all positive and display
	use do while

*/
#include <iostream>
using namespace std;
int main() {

	// declare variable
	int num, product = 1, count = 0;

	// ?? do while loop to ask input, positive value to continue
	do{

	// ask input
	cout << "Enter a number: ";
	cin >> num;
	
	// calculate for product *
	if (num > 0) {
		product *= num;
		count++ ;
	}
	}while (num > 0);
	


	// display result
	if (count > 0)
		cout << "The product of all your positive numbers is "
		<< product << "." << endl;
	else
		cout << "You have entered a number less than or equal to zero." << endl;
	return 0;
}
