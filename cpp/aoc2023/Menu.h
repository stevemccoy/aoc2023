#pragma once

using namespace std;

#include <vector>
#include "Problem.h"

class Menu
{
public:
	Menu();
	~Menu();
	// Load menu options.
	void loadOptions();
	// Present the menu options on stdout.
	void display();
	// Get user choice from stdin.
	int readUserChoice();
	// Run the event loop.
	void run();

private:
	vector<Problem*> problems;

	// Determine if given option is appropriate input for a choice from the menu.
	bool validOption(int option);

};

