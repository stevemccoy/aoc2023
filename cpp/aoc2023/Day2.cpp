#include "Day2.h"
#include "common.h"
#include <iostream>
#include <map>

void Day2::execute()
{
	cout << "Part 1. Test Input..." << endl;
	part1("input/test2.txt");
	cout << "Part 1. Actual Input..." << endl;
	part1("input/input2.txt");
	cout << "Part 2. Test Input..." << endl;
	part2("input/test2.txt");
	cout << "Part 2. Actual Input..." << endl;
	part2("input/input2.txt");
}

void Day2::part1(const char* fileName) const {
	const map<string, int> limits = { {"red",  12}, {"green", 13}, {"blue", 14} };
	auto games = read_game_counts(fileName);
	int sum = 0;
	for (auto game : games) {
		bool over = false;
		for (auto p : limits) {
			if (game.second[p.first] > p.second) {
				over = true;
				break;
			}
		}
		if (!over) {
			sum += game.first;
		}
	}
	cout << "Input from file " << fileName << " - Sum is " << sum << endl;
}

map<int, map<string, int>> Day2::read_game_counts(const char* fileName) const {
	map<int, map<string, int>> result;
	auto lines = read_input_file(fileName);
	int gameId, sum = 0;
	for (string line : lines) {
		auto split1 = split_delim(line, ':');
		sscanf_s(split1[0].c_str(), "Game %d", &gameId);
		map<string, int> game;
		int count;
		char colour[80] = "";
		game.clear();
		for (auto s1 : split_delim(split1[1], ';')) {
			for (auto s2 : split_delim(s1, ',')) {
				sscanf_s(s2.c_str(), "%d %s", &count, colour, (unsigned)_countof(colour));
				if (game.find(colour) == game.end()) {
					game[colour] = count;
				}
				else if (count > game[colour]) {
					game[colour] = count;
				}
			}
		}
		result[gameId] = game;
	}
	return result;
}

int Day2::gamePower(map<string, int>& game) const {
	int r = game["red"];
	int g = game["green"];
	int b = game["blue"];
	return r * g * b;
}

void Day2::part2(const char* fileName) const {
//	const map<string, int> limits = { {"red",  12}, {"green", 13}, {"blue", 14} };
	auto games = read_game_counts(fileName);
	int sum = 0, power = 0;
	for (auto& game : games) {
		power = gamePower(game.second);
		sum += power;
	}
	std::cout << "Input from file " << fileName << " - Sum is " << sum << endl;
}

