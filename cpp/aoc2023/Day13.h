#pragma once
#include "Problem.h"
#include <vector>

class Day13 : public Problem
{
public:
	Day13() {
		init("Day 13", "Point Of Incidence");
	}
	void execute();
	void part1(const char* fileName);
	void part2(const char* fileName);

private:
	bool process_frame(std::vector<std::string>& lines, int& score, int smudges);
	int compare_rows(string row1, string row2);

};
