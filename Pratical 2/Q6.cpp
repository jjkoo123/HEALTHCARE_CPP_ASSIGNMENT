#include <iostream>
#include <iomanip> // for std::fixed and std::setprecision

using namespace std;

int main()
{
	const int MAX_LOAN_DAYS = 14;
	const double FINE_PER_DAY_PER_BOOK = 0.20;

	int numBooks, loanDays;

	cout << "------------------\n";
	cout << "BOOK LOAN SYSTEM\n";
	cout << "------------------\n";

	cout << "Enter the number of books  : ";
	cin >> numBooks;

	cout << "Enter the days of the loan : ";
	cin >> loanDays;

	cout << "------------------------------\n";

	int overdueDays = loanDays - MAX_LOAN_DAYS;
	if (overdueDays < 0)
		overdueDays = 0;

	double fine = overdueDays * numBooks * FINE_PER_DAY_PER_BOOK;

	cout << "Days overdue                : " << overdueDays << endl;
	cout << "Fine                        : RM " << fixed << setprecision(2) << fine << endl;

	return 0;
}
