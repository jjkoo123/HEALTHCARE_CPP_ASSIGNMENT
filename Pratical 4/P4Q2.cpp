/*
	P4Q2: Question ask to get temperature for user, then decide the activity based on temperature
	Need to write psudocode also
*/

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;
int main()
{
	// declare variable to store temperature
	int temp;

	// Prompt and get temperature
	cout << "Enter Temperature : ";
	cin >> temp;

	// ?? if statement to filter based on temp, display the result
	if (temp > 85)
	{
		cout << "You can go Swimming" << endl;
	}
	else if (temp > 70 && temp <= 85)
	{
		cout << "You can go Tennis" << endl;
	}
	else if (temp < 32 && temp <= 70)
	{
		cout << "You can go Golf" << endl;
	}
	else if (temp > 0 && temp <= 32)
	{
		cout << "You can go Skiing" << endl;
	}
	else if (temp <= 0)
	{
		cout << "You can go Dancing" << endl;
	}

	return 0;
}

/*
Pseudocode

START
	Variable definition and initialization
	Read temp
	IF (temp > 85)
		Print �Swimming�
	ELSE IF (temp > 70 AND temp <= 85)
		Print �Tennis�
	ELSE IF (temp > 32 AND temp <= 70)
		Print �Golf�
	ELSE IF (temp > 0 AND temp <= 32)
		Print �Skiing�
	ELSE
		Print �Dancing�
	ENDIF
END

*/
