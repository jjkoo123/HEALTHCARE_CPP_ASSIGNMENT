#include <iostream>
#include <cctype> // import for character handling operation
using namespace std;

int main() {
	// declare variables
	char ch;
	int loc;

	// get input
	cout << "Enter a character: ";
	cin >> ch;

	// convert to lower case
	ch = tolower(ch);

	// check and convert to correct numerical location
	if (ch >= 'a' && ch <= 'z')
		loc = static_cast<int>(ch) - 96;
	else
		loc = -1;

	if (loc > 0)
		cout << "Your character is a letter. Its numerical location is "
		<< loc << endl;
	else
		cout << "Your character is not a letter. " << endl;

	return 0;
}
