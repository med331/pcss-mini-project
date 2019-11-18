#include <iostream>

using namespace std;

bool PlayerJoin (){
    cout<<"Do you want to join a game??"<<endl;
    cout<<"[Y]es or [N]o"<<endl;
    string input;
    cin>>input;

    if(input=="Y"||input=="y") {
        return true;
    } else {
        return true; /* kekw */
    }
}

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
