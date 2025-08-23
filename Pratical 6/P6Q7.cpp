#include<iostream>
#include<cctype> // import for character handling operation
#include<string>
using namespace std;

int main() {
	// declare variables
	char c;
	string text = "I am a good student. My Lecturer loves me very much. Thank you";
	int i, count = 0;

	cout << text << endl << endl;

	// get input
	cout << "Which character to count? ";
	cin >> c;

	// ?? convert to lower case and use for loop to check
	for (i = 0; i < text.length(); i++)
	{
		if (tolower(text[i]) == tolower(c))
			count++;
	}

	// display result
	if (count > 0)
		cout << "There are " << count << " character \'"
		<< c << "\' in the text." << endl;
	else
		cout << "There isn't any character \'" << c
		<< "\' you requested in the text." << endl;
	return 0;
}
