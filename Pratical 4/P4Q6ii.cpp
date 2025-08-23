/*
	P4Q6i: get input from user and calculate the income + commission based on different level
	using multi if and draw flowchart

*/
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
	// declare variables
	double monthlySales, monthlyIncome = 0;

	// Prompt and get monthly sales
	cout << "Enter monthly sales : RM ";
	cin >> monthlySales;

	// ?? multi if statement
	// calculate based on different categories
	if (monthlySales >= 50000)
	{
		monthlyIncome = 375 + (monthlySales * 0.16);
	}
	else if (monthlySales < 50000 && monthlySales >= 40000)
		monthlyIncome = 350 + (monthlySales * 0.14);
	else if (monthlySales < 40000 && monthlySales >= 30000)
	{
		monthlyIncome = 325 + (monthlySales * 0.12);
	}
	else if (monthlySales < 30000 && monthlySales >= 20000)
	{
		monthlyIncome = 300 + (monthlySales * 0.09);
	}
	else if (monthlySales < 20000 && monthlySales >= 10000)
	{
		monthlyIncome = 250 + (monthlySales * 0.05);
	}
	else
	{
		monthlyIncome = 200 + (monthlySales * 0.03);
	};

	// display result
	cout << "Monthly Income is : RM " << fixed << setprecision(2) << monthlyIncome << endl;

	return 0;
}
