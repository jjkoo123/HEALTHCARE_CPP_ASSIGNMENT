/*
	P4Q7: check the value is what then display the message using switch statement

*/
#include <iostream>
using namespace std;

int main()
{
	// declare variable
	int flag;

	// Prompt and get flag
	cout << "Enter flag value : ";
	cin >> flag;

// ?? Use switch statement to filter and display
switch (flag)
{
case 1:
	cout << "Hot" << endl;
	break;

case 2:
	cout << "Warm" << endl;
	break;

case 3:
	cout << "Cold" << endl;
	break;
default:
	cout << "Out of range" << endl;
	break;
}

	return 0;
}
