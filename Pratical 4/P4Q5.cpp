/*
	P4Q5: Read the maximim capacity of a room and number of people attend
	if the number of people is less than the max capacity, then list the seats available
	if the number of people is more than the max capacity, then list how many people need to be excluded

*/

#include <iostream>
using namespace std;

int main() {
	// Declare variables
	int roomCapacity, noPeople, total = 0;

	// Prompt and get maximum room capacity
	cout << "Enter maximum room capacity : ";
	cin >> roomCapacity;

	// Prompt and get total number of people attend
	cout << "Enter total number of people attending the meeting: ";
	cin >> noPeople;

	cout << endl;

	//  ?? If statement
	// if number of people is less than the max, then calculate seat available
	// if number of people is greater than the max, then calculate number of people to be exlcuded
	if(noPeople <= roomCapacity){
		total = roomCapacity - noPeople;
		if(total > 0) {
			cout << "Seats avalable: " << total << endl;
		} else {
			cout << "No more seats available." << endl;
		}
	}

	if(noPeople > roomCapacity){
		cout << "Violation of fire law regulations" << endl;
		total = noPeople - roomCapacity;
		
		if(total > 0){
			cout << "People must be excluded: " << total << endl;
		}else { 
				cout << "No people to be excluded." << endl;
			// No need to exclude anyone if total is not greater than 0
			// This is just a safety check, as the previous condition ensures total > 0
			// but it's good practice to handle all cases.
			}
		}
	


	cout << endl;
	return 0;
	}

/*
Pseudocode
START
	Variable definition and initialization
	Read roomCapacity, noPeople
	IF (noPeople <= roomCapacity)
		total = roomCapacity � noPeople
		IF (total > 0)
			Print �Seats available�
		Else
			Print �No more seats available�
		ENDIF
	ELSE
		Print �Violation of fire law regulations�
		total = noPeople � roomCapacity
		IF (total > 0)
			Print �People must be excluded�
		ENDIF
	ENDIF
END

*/
