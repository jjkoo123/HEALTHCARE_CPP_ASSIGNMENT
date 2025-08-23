#include <iostream>
#include <iomanip>
// ?? import for math operation
#include <cmath>
using namespace std;

int main() {
	int i;
	cout << "Number\tSquare\tCube" << endl;
	// ?? use for loop to display square and cube by using pow()
	for(i = 1; i <= 10; i++){
		cout << i << "\t" << pow(i, 2) << "\t" << pow(i, 3) << endl;
	}

	return 0;
}
