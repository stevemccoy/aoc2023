// aoc2023.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Menu.h"

int main()
{
    std::cout << "Advent of Code 2023.\n";
    Menu mainMenu;
    mainMenu.loadOptions();
    mainMenu.run();
    cout << "Finished - Bye." << endl;
}
