#include <iostream>

using namespace std;

/**
 *  Ask the player to either hit or stand
 */
bool PlayerHit (){
    /* prompt the user */
    cout<<"Do you wish to [H]it or [S]tand?"<<endl;
    
    /* listen for input */
    string input;
    cin>>input;
    
    /* parse input */
    if(input=="H"||input=="h") {
        return true;
    }
    else {
        return false;
    }
}
