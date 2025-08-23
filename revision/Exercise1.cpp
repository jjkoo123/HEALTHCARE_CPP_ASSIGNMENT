#include <iostream>
#include <iomanip>
using namespace std;

int main() {

	// declare variables
	char userReply;
	int selectedItem, qty;
	double price = 0.0, totalPrice;
	string itemName;

	cout << "Welcome to Food Ordering System";

	// do while to ask for input and repeat to handle different orders
	do {
		cout << "\n1. KFC - RM 50.00\n";
		cout << "2. McDonald - RM 150.00\n";
		cout << "3. Burger King - RM 100.00\n";
		cout << "4. Domino - RM 20.00\n";
		cout << "5. Pizza - RM 200.00\n";
		cout << "What food you want to purchase (1-5) > ";
		cin >> selectedItem;

		// if statement to check valid or not
		if (selectedItem >= 1 && selectedItem <= 5) {
			cout << "Enter the quantity that you want to purchase > ";
			cin >> qty;

			// switch statement to assign price and name
			switch (selectedItem) {
			case 1:
				price = 50.00;
				itemName = "KFC";
				break;
			case 2:
				price = 150.00;
				itemName = "McDonald";
				break;
			case 3:
				price = 100.00;
				itemName = "Burger King";
				break;
			case 4:
				price = 20.00;
				itemName = "Domino";
				break;
			case 5:
				price = 200.00;
				itemName = "Pizza";
				break;
			}

			// calculate total price
			totalPrice = qty * price;

			//display output
			cout << "\nFood Ordering System" << endl;
			cout << "Order Summary" << endl;
			cout << "----------------" << endl;
			cout << left << setw(15) << "Item Name" << setw(15) <<  "Unit Price" << setw(15) 
				<< "Quantity" << setw(11) << "Total Price" << endl;
			cout << left << fixed << setprecision(2) << setw(15) << itemName << setw(15) << 
				price << setw(15) << qty << setw(11) << setfill('*') << totalPrice <<endl;

			cout << "\nThanks for your support."<< endl;
		}else{
			cout << "Invalid, please try later\n";
			
		}

		//ask want to continue or not
		cout << "Are you want to continue (Y or N) > ";
		cin >> userReply;

	} while (userReply == 'Y' or userReply == 'y');
	return 0;
}