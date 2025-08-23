#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main()
{
    char userReply;
    int num, quantity;
    double price, totalPrice;
    string foodname;

    do
    {
        cout << "Welcome to Food Ordering System" << endl;
        cout << "1. KFC - RM50.00" << endl;
        cout << "2. McDonald's - RM150.00" << endl;
        cout << "3. Burger King - RM100.00" << endl;
        cout << "4. Domino - RM20.00" << endl;
        cout << "5. Pizza - RM200.00" << endl;

        cout << "What food you want to order? (1-5): ";
        cin >> num;

        if (num >= 1 && num <= 5)
        {
            cout << "Enter the quantity that you want to purchase : ";
            cin >> quantity;

            switch (num)
            {
            case 1:
                foodname = "KFC";
                price = 50.00;
                break;

            case 2:
                foodname = "McDonald's";
                price = 150.00;
                break;

            case 3:
                foodname = "Burger King";
                price = 100.00;
                break;

            case 4:
                foodname = "Domino";
                price = 20.00;
                break;

            case 5:
                foodname = "Pizza";
                price = 200.00;
                break;
            }

            totalPrice = price * quantity;

            cout << "Food Ordering System" << endl;
            cout << "Order Summary" << endl;
            cout << "- - - - - - - - - - - - - - - - " << endl;
            cout << "Item name " << setw(15) << "Unit Price" << setw(15) << "quantity" << setw(15) << "Total Price" << endl;
            cout << foodname << setw(15) << price << setw(15) << quantity << setw(15) << totalPrice << endl;
        }
        else
        {
            cout << "Invalid , please try later." << endl;
        }

        cout << "Are you want to continue? (Y or N): ";
        cin >> userReply;

    } while (userReply == 'Y' || userReply == 'y');

    return 0;
}
