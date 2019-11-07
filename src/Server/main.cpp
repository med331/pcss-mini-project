//Server Main
#include <iostream>
#include "game_logic.cpp"
#include "session_management.cpp"

using namespace std;


int main()
{
    ConnectionManager manager;
    game_logic object;
    object.updateGame();
    return 0;
}
