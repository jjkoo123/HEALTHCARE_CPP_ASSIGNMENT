#include <iostream>
#include <cstring> // import for null terminated function
#include <cctype> // import for character handling operation
using namespace std;

int main() {
	// declare variables
	int i = 0;
	char text[50];

	// get input
	cout << "Enter a sentence: ";
	cin.getline(text, 50, '\n');

	// use while loop to convert all as uppercase
	while (i < strlen(text))
	{
		text[i] = toupper(text[i]);
		i++;
	}

	// display result
	cout << "The converted sentence in uppercase is as below: \n" << text << endl;
	return 0;
}
