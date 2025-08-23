#include <iostream>
#include <string>
using namespace std;

int main() {
	// declare variables
	string text;
	int length, i;

	// get input
	cout << "Enter your sentence: ";
	getline(cin, text);

	cout << "Your sentence printed backward: ";

	// display in backward mode
	for (i = text.length() - 1; i >= 0; i--)
		cout << text.at(i);
	cout << endl;
	return 0;
}
