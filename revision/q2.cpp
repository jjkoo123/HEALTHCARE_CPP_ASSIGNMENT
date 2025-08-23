#include <iostream>
#include <iomanip>
#include <string>
#define MAX_DAYS 7

using namespace std;

int main()
{

    char choise;
    int book, days, quantity = 1;

    cout << "Welcome to Book Borrowing System" << endl;

    do
    {
        cout << "1. How to get a GF ???" << endl;
        cout << "2. What Single Dog Can Do ???" << endl;
        cout << "3. How to Pass test without Revision " << endl;
        cout << "4. Dream big, Think to much." << endl;
        cout << "5. How to control your Life ???" << endl;

        cout << "The maximum days to borrow is 7 and quantity is 1" << endl;
        cout << "What book you want to borrow? (1-5)> " << endl;
        cin >> book;

        if (book >= 1 && book <= 5){
            cout << "You want to borrow for how many days??? > " << endl;
            cin >> days;

            if (days < 1 || days > MAX_DAYS)
            {
                cout << "Hello bro, enter 1 -> 7 lah" << endl;
            }
            else
            {
                cout << "Book Borrowing System" << endl;
                cout << "Summary" << endl;
                cout << "- - - - - - - - - - - - - - - - " << endl;
                cout << "Book No." << setw(20) << "Quantity" << setw(20) << "Number of Days" << endl;
                cout << book << setw(20) << quantity << setw(20) << days << endl;
            }
        }
            
        else
        {
            cout << "Hello, invalid input lah...you...cannot lah" << endl;
        }

        cout << "Are you want to continue? (Y/N): ";
        cin >> choise;
    } while (choise == 'Y' || choise == 'y');
    return 0;
}