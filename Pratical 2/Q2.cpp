/*
	Question ask you to  manipulator setw() to display the diamond shape
*/

#include <iostream>
#include <iomanip>
using namespace std;

int main() {

	// use setw() to create empty space
	cout << setw(3) << "*" << endl;
	cout << setw(2) << "*" << setw(2) << "*" << endl;
	cout << "*" << setw(4) << "*" << endl;;
	cout << setw(2) << "*" << setw(2) << "*" << endl;
	cout << setw(3) << "*";
	return 0;

}