#pragma once
#include "Problem.h"
#include <vector>

class Day4 : public Problem
{
public:
	Day4() { init("Day 4", "Scratchcards");	}
	void execute();
	void part1(const char* fileName);
	void part2(const char* fileName);
private:
	int countMatches(string line);
	int score(unsigned int numMatches);
	vector<int> extract_numbers(string s);
};
