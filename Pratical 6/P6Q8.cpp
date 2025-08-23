#include <iostream>
// ?? import for character handling operation
#include <cctype>
#include <string>
using namespace std;

int main()
{
	// declare variables
	string text;
	int upper = 0, lower = 0, space = 0, digit = 0, other = 0, i;

	// get input
	cout << "Enter a line of text: ";
	getline(cin, text);

	cout << "\nThe text you have entered is: " << text << endl;

	// ?? use for loop to check for uppercase, lowercase, spacing, digit and other
	for (i = 0; i < text.length(); i++)
	{
		if (isupper(text.at(i)))
			upper++;
		else if (islower(text.at(i)))
			lower++;
		else if (isspace(text.at(i)))
			space++;
		else if (isdigit(text.at(i)))
			digit++;
		else
			other++;
	}

	// display result
	cout << "\nText\t\tCount\n";
	cout << "----------------------\n";
	cout << "Upper case\t" << upper << endl;
	cout << "Lower case\t" << lower << endl;
	cout << "Space\t\t" << space << endl;
	cout << "Digit\t\t" << digit << endl;
	cout << "Others\t\t" << other << endl;

	return 0;
}
