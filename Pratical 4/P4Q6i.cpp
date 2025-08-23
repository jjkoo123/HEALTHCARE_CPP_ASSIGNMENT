/*
	P4Q6i: get input from user and calculate the income + commission based on different level
	using nested if else and draw flowchart

*/

#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
	// declare variables
	double monthlySales, income = 0;

	// Prompt and get monthly sales
	cout << "Enter monthly sales : RM ";
	cin >> monthlySales;

	// ?? nested if else
	// calculate based on different categories
	if (monthlySales >= 10000)
	{
		if (monthlySales >= 50000)
		{
			income = 375 + (monthlySales * 0.16);
		}
		else
		{
			if (monthlySales >= 40000)
			{
				income = 350 + (monthlySales * 0.14);
			}
			else
			{
				if (monthlySales >= 30000)
				{
					income = 325 + (monthlySales * 0.12);
				}
				else
				{
					if (monthlySales >= 20000)
					{
						income = 300 + (monthlySales * 0.09);
					}
					else
					{
						income = 250 + (monthlySales * 0.05);
					}
				}
			}
		}
	}
	else
	{
		income = 200 + (monthlySales * 0.03);
	}

	// display result
	cout << "Monthly Income is : RM " << fixed << setprecision(2) << income << endl;
	return 0;
}
