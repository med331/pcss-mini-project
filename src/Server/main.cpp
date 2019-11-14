//Server Main
#include <iostream>
#include "game_logic.h"
#include "session_management.cpp"

using namespace std;


int main()
{
    ConnectionManager manager;
    game_logic object;
    object.updateGame();
        manager.ServerUpdate();
    return 0;
}
