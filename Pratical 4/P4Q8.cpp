/*
	P4Q8: ask for colour from user, then use switch statement to display

*/
#include <iostream>
using namespace std;

int main()
{
	// delcare variables
	char colour;
	int level;

	// Prompt and get colour
	cout << "Enter code for colour : ";
	cin >> colour;

	// ?? Write switch statement based on colour and display result
	switch (colour)
	{
	case 'r' | 'R':
		cout << "Red" << endl;
		cout << "Enter level (1-3): " << endl;
		cin >> level;
		
		switch (level)
		{

		case 1:
			cout << "Caution" << endl;
			break;

		case 2:
			cout << "Danger" << endl;
			break;

		case 3:
			cout << "Critical" << endl;
			break;

		default:
			cout << "Invalid" << endl;
			break;
		}

		break;

	case 'g' | 'G':
		cout << "Green" << endl;

	case 'b' | 'B':
		cout << "Blue" << endl;
		break;

	default:
		cout << "Black" << endl;
		break;
	}

	return 0;
}
