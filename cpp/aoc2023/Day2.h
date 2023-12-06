#pragma once
using namespace std;

#include "Problem.h"
#include <map>

class Day2 : public Problem
{
public:
	Day2() {
		init("Day 2: Cube Conundrum", "Cube Conundrum");
	}
	void execute();
	void part1(const char* fileName) const;
	void part2(const char* fileName) const;
private:
	map<int, map<string, int>> read_game_counts(const char* fileName) const;
	int gamePower(map<string, int>& game) const;
};
