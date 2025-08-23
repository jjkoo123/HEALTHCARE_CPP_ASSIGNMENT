/*
	P5Q10: display the multiplication table based on user input
*/
#include <iostream>
#include <iomanip>
using namespace std;
int main()
{

	// declare variable
	int num;

	// ask for input - number
	cout << "Multiplication table of what number? ";
	cin >> num;

	// use for loop to display the multiplication table based on the input
	for (int i = 1; i <= 12; i++)
		cout << num << " X " << i << " = " << setw(5) << num * i << endl;
	return 0;
}

