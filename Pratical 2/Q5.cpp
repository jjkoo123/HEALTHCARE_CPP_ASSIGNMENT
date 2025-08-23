/*
	Question ask you to read characters from user and convert to number based on ASCII table
*/

#include <iostream>
using namespace std;

int main() {

	// define variable
	char character;

	// prompt and get user input
	cout << "Enter a character:" << endl;
	cin >> character;

	// ?? convert user input - character to integer based on ASC II format
    // static_cast is to avoid compile warning message
    int decimalValue = static_cast<int>(character); // convert character to integer
	


	// extra info: if question ask to convert integer to character, then write the code as static_cast<char>(userInput)

	//display result
	cout << "The decimal value for " << character << " is " << decimalValue << endl;
	return 0;

}