/*
	P5Q12: get temparature from user then check the category and display at the end

*/
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
	// declare variables
	int hot = 0, pleasant = 0, cold = 0;
	int temp = 1, total = 0;
	double avg;

	// ask for input - temperature
	cout << "Enter temperature: ";
	cin >> temp;

	// if statement to filter, prompt error message if negative or zero
	if (temp > 0)
	{
		// ?? while loop to check, positive to continue
		while (temp > 0)
		{

			// if statement to filter temperature category
			if (temp >= 85)
			{
				hot++;
				cout << "Hot Day" << endl;
			}
			else if (temp >= 60 && temp <= 84)
			{
				pleasant++;
				cout << " Pleasant Day" << endl;
			}
			else if (temp >= 1 && temp < 60)
			{
				cold++;
				cout << "Cold Day" << endl;
			}

			// calculate the total positive temperature
			total += temp;
			// ask input again, positive to continue
			cout << "Enter temperature: ";
			cin >> temp;
		}

		// display result
		avg = static_cast<double>(total) / (hot + pleasant + cold);
		cout << "\nNumber of hot days are: " << hot << endl;
		cout << "Number of pleasant days are: " << pleasant << endl;
		cout << "Number of cold days are: " << cold << endl;
		cout << "Average temperature is: " << fixed << setprecision(1)
			 << avg << endl;
	}
	else
		cout << "You have entered an invalid temperature. " << endl;
	return 0;
}
