#include <iostream>
 // ?? import for math operation
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
	cout << "Number\tSquareRoot\n";
	cout << fixed << setprecision(4);

	// ?? use for loop to display for square root by using sqrt()
	for(int i = 0; i <= 20; i++){
		cout << i << "\t" << sqrt(i) << endl;
	}

	return 0;
}
