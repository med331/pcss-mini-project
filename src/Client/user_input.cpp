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
        return false;
    }
}

bool PlayerHit (){
    cout<<"Do you wish to Hit or Stand?"<<endl;
    string input;
    cin>>input;

    if(input=="Hit") {
        return true;
    } else {
        return false;
    }
}
