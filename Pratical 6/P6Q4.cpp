#include <iostream>
#include <cctype> // ?? import for character handling operation
using namespace std;

int main() {
	// declare variables
	char ch;

	// get input
	cout << "Enter a character (9 to stop) : ";
	cin >> ch;

	// while loop to repeat / stop
	while (ch != '9') {
		// ?? convert character to lower case
		ch = tolower(ch);

		// ?? check is alphabet or not
		if(isalpha(ch)){
			if(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u'){
				cout << ch << " is a vowel" << endl;
			}else{
				cout << ch << " is a consonant" << endl;
			}
		}else{
			cout << ch << " is not an alphabet" << endl;
		}


		cout << "\nEnter a character (9 to stop) : ";
		cin >> ch;
	}
	return 0;
}
