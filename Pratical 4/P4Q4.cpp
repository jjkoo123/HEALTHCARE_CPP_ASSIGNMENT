/*
	P4Q4: Ask the get the hourly wage and working hours from user,
	calculate OT with additional 50% if working hours is > 40 per week
	calculate the total wages

*/

#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
	// declare const variable
	const int MAX_HOURS = 40;
	// declare variables
	int hoursWork;
	double wages, totalWages;

	// ?? prompt and get working hours and wages
	cout << "Enter your hourly wage: ";
	cin >> wages;
	cout << "Enter hours: ";
	cin >> hoursWork;

	// ?? Use if statement to filter
	// if more than max working hours, then calculate extra 50%
	if (hoursWork > MAX_HOURS)
	{
		int overtimes_hours = hoursWork - MAX_HOURS; // calculate extra hours
		totalWages = (MAX_HOURS * wages) + (overtimes_hours * wages * 1.5);
	}

	// else normal calculation + hour checking
	else
	{
		totalWages = hoursWork * wages;
	}

	cout << fixed << setprecision(2);
	cout << "Total wages: RM" << totalWages << endl;

	if (hoursWork < MAX_HOURS)
	{
		cout << "You are underworked." << endl;
	}

	return 0;
}
