#include <iostream>

using namespace std;

// Takes input from player if they want to hit or stand
bool PlayerHit (){
    cout<<"Do you wish to [H]it or [S]tand?"<<endl;
    string input;
    cin>>input;

    if(input=="H"||input=="h") {
        return true;
    }
    else {
        return false;
    }

}
