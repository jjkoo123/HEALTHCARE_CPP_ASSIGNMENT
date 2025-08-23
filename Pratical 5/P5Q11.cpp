/*
	P5Q11: based on the user input to calculate the sum

*/
#include<iostream>
using namespace std;
int main()
{
	// declare variables
	int n, i, j, sum = 0;

	// ask for n
	cout << "Please enter the value for n : ";
	cin >> n;

	// ?? for nested for loop to calculate sum based on n
	for(int i =1; i <= n ; i++){
		sum += i;
	}

	// Display result
	cout << "The sum is: " << sum << endl;
	return 0;
}
