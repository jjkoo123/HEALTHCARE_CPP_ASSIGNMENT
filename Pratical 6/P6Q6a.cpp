#include <iostream>
#include <string>
// ?? import for character handling operation
#include <cctype>
using namespace std;

int main()
{
	// declare variables
	int i = 0;
	string text;

	// get input
	cout << "Enter a sentence: ";
	getline(cin, text);

	// ?? while loop to check the length of string
	while (i < text.length())
	{
		// convert to uppercase
		text.at(i) = toupper(text.at(i));
		i++;
	}

	// display result
	cout << "The converted sentence in uppercase is as below: \n" << text << endl;
	return 0;
}
