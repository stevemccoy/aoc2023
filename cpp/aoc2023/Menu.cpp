#include "Menu.h"
#include "Day1.h"
#include <iostream>
#include <string>

Menu::Menu()
{
	problems.clear();
}

Menu::~Menu()
{
	for (Problem* p : problems) {
		delete p;
	}
	problems.clear();
}

void Menu::loadOptions()
{
	// Find out the full list of Problems.
	Day1 d1;
	// Sort and store them.
	problems.push_back(new Day1());
}

void Menu::display()
{
	cout << "\nMenu:" << endl;
	int i = 1;
	for (Problem* p : problems) {
		cout << i << ". " << p->title() << " - " << p->description() << endl;
	}
	cout << endl;
}

int Menu::readUserChoice()
{
	int option = 0;
	cout << "Option (-1 to quit): ";
	cin >> option;
	if (validOption(option)) {
		return option;
	}
	return option;
}

void Menu::run()
{
	int option = 0;
	while (true) {
		display();
		option = readUserChoice();
		if (option == -1) {
			break;
		}
		cout << endl;
 		problems[option - 1]->execute();
	}
}

bool Menu::validOption(int option)
{
	if ((option == -1) || ((option >= 1) && (option <= problems.size()))) {
		return true;
	}
	return false;
}
