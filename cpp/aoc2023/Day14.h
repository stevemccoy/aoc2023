#pragma once
#include "Problem.h"
#include<vector>

class Day14 : public Problem
{
public:
	Day14() {
		init("Day 14", "Parabolic Reflector Dish");
		numRows = numCols = 0;
		grid = nullptr;
	}
	void execute();
	void part1(const char* fileName);
	void part2(const char* fileName);

private:
	int numRows;
	int numCols;
	char* grid;

	static const long big_number = 1000000000L;

	char get(int c, int r) { return grid[c + r * numCols]; }
	void set(int c, int r, char v) { grid[c + r * numCols] = v; }

	void process_lines(const std::vector<std::string>& lines);
	void roll_north();
	void roll_south();
	void roll_west();
	void roll_east();

	void roll_one_cycle();
	void roll_n_cycles(long n);

	int calculate_load();

};
