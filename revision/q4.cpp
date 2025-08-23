#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main(){

    char typeOfGadget ;
    int quantity;
    double price, totalPrice, discountAmount, netAmount;

    while(true){
        cout << "Type of Gadget Selected: ";
        cin >> typeOfGadget;
        typeOfGadget = toupper(typeOfGadget);

        if (typeOfGadget == 'X'|| typeOfGadget == 'Y' || typeOfGadget == 'Z'){
            cout << "Price (RM): ";
            cin >> price;
            cout << "Quantity: " << endl;
            cin >> quantity;
        }else{
            cout << "Wrong type, please Enter again" << endl;
        }
        
        switch (typeOfGadget)
        {
        case 'X' :
            if(price >= 5000)
                discountAmount = 0.35 * price;
            else
                discountAmount = 0.30 * price;
            
            break;
        
        case 'Y' :
        if(price >= 8000)
                discountAmount = 0.25 * price;
            else
                discountAmount = 0.20 * price;
            
            break;

        case 'Z' :
        if(price >= 1000)
                discountAmount = 0.15 * price;
            else
                discountAmount = 0.05 * price;
            
            break;
        
        }
        totalPrice = price * quantity;
        netAmount = totalPrice - (totalPrice - discountAmount);

        cout << "Order Summary" << endl;
        cout << "- - - - - - - - - - - - - - - " << endl;
        cout << "Type " << setw(15) << "Total Price (RM) " << setw(15) << "Net Amount (RM)" << endl;
        cout << typeOfGadget << fixed << setprecision(2) << setw(15) << totalPrice << setw(20)  <<  netAmount << endl;

    }
}


