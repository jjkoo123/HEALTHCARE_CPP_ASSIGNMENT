/*
	P4Q1: Question ask to read customer name, pruchase amount and tax code.
	Calculate the sales tax and total amount due based on tax code
	display customer anme, pruchase amount, sales tax and total amount due

*/

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

int main()
{
	// ?? Declare variable - store name
	string customerName;
	// ?? Declare variable - store tax code number
	int tax_code;
	// ?? Declare variables - store purchaseAmount, salesTaxAmount and totalAmount
	double purchaseAmount, sales_tax, totalAmount;

	// ?? Prompt and get customer full name
	cout << "Enter your name: ";
	getline(cin, customerName); // Use getline to allow spaces in the name

	// ?? Prompt and get purchase amount
	cout << "Enter the purchase amount: RM ";
	cin >> purchaseAmount;

	// ?? Prompt and get tax code number
	cout << "Enter the tax code (0-4): ";
	cin >> tax_code;

	// ?? Write switch statement - filter based on taxCode and calculate
	switch (tax_code)
	{
	case 0:
		sales_tax = 0.00;
		break;
	case 1:
		sales_tax = purchaseAmount * 0.06;
		break;
	case 2:
		sales_tax = purchaseAmount * 0.10;
		break;
	case 3:
		sales_tax = purchaseAmount * 0.16;
		break;
	case 4:
		sales_tax = purchaseAmount * 0.06;
		break;
	default:
		cout << "Invalid tax code number." << endl;
		break; // Invalid tax code
	}

	// display output
	cout << "********************************************" << endl;

	// ?? Use if statement to check tax_code correct or not
	// If wrong, display wrong tax code
	if (tax_code < 0 || tax_code > 4)
	{ // 只可以用 ||  没有==> or =>
		cout << "Invalid tax code, Please enter the valid tax code (0-4)." << endl;
		return 1; // if want to return need to use while loop
	}
	else
	{
		totalAmount = purchaseAmount + sales_tax;

		// If correct, calculate the totalAmount and display result
		cout << fixed << setprecision(2); // Set precision for monetary values
		cout << "Customer Name: " << customerName << endl;
		cout << "Purchase Amount: RM " << purchaseAmount << endl;
		cout << "Sales Tax Amount: RM " << sales_tax << endl;
		cout << "Total Amount Due: RM " << totalAmount << endl;
	}

	return 0;
}
