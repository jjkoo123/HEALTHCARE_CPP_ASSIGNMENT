#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main(){
    
    char  type;
    int price, quantity;
    double totalPrice, discount , netAmount;

    do{
        cout << "Type of Meal Selected: ";
        cin >> type ;

        //if else
        if(type != 'A' && type != 'B' && type != 'C'){
            cout << "wrong type, please enter again" << endl;
        }

    }while(type != 'A' && type != 'B' && type != 'C');

    cout << "Price(RM): ";
    cin >> price;
    cout << "Quantity: ";
    cin >> quantity;


    //nested if else
    if(type == 'A'){
        if(price >= 5000){
            discount = 0.35
        }else {
            discount = 0.30
        }

    }else if(type == 'B'){
        if(price >= 8000){
            discount = 0.25
        }else {
            discount = 0.20
        }

    }else if(type == 'C'){
        if(price >= 1000){
            discount = 0.15
        }else {
            discount = 0.05
        }
    }

    // calculate total price
    totalPrice = price * quantity;
    netAmount = totalPrice - (totalPrice * discount);

    

    return 0;
}