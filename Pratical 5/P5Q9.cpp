/*
	P5Q9: Ask for a list of integer from user, stop value is -999
	then check which one is smallest and entered how many times

*/

#include <iostream>
#define SENTINEL -999 // declare constant
using namespace std;

int main()
{
	// declare variables
	int smallest, num, count;

	// get input
	cout << "Please enter an integer : ";
	cin >> num;

	// assume the first input is the smallest
	smallest = num;
	count = 1;
	
	// ?? ask for next input using for loop + cin
	// check again is the latest number is smaller?
	// ask for next input or stop
	
	while (cin >> num && num != SENTINEL){
		if(num < smallest){
			smallest = num;
			count = 1;
		}else if (num == smallest){
			count++;
		}
	cout << "Enter next integer or -999 to stop : ";
	}




	// display the smallest value based on all input
	cout << "The smallest value is " << smallest << " and it was entered "
		<< count << " time(s)." << endl;
	return 0;
}
