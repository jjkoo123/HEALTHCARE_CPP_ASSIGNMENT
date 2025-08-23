/*
	Question ask you to get floating number from user and perform caculation using #define value
*/

#include <iostream>
#include <iomanip>
// ?? another method to define constant using #define
#define PI 3.1415
using namespace std;

int main() {

	// ?? define variables
    double radius, circle ;
	// avoid to use all uppercase as normal variable name, all uppercase is for constant variable


	// ?? prompt and get input
    cout << "Enter radius: ";
    cin >> radius;

	// ?? perform calculation
	circle = 2 * PI * radius;

	// ?? display the answer with 2 decimal points
    cout << "The result is: " << fixed << setprecision(2) << circle << endl;

	return 0;

}