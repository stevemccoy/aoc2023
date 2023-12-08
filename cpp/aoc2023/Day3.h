#pragma once
#include "Problem.h"

#include <vector>
#include <map>

static int nextGearId = 1;

class Part {
public:
	Part(int v, size_t r, size_t cs, size_t ce) : row(r), startCol(cs), endCol(ce), value(v) {}
	size_t row;
	size_t startCol;
	size_t endCol;
	int value;
};

class Gear {
public:
	Gear(size_t c, size_t r) : id(nextGearId++), row(r), col(c) {}
	int id;
	size_t row;
	size_t col;

};

class Day3 : public Problem
{
public:
	Day3() {
		init("Day 3", "Gear Ratios");
	}
	void execute();
	void part1(const char* fileName);
	void part2(const char* fileName);

	size_t offset(size_t col, size_t row) {
		return row * num_cols + col;
	}

	void set(size_t col, size_t row, char value) {
		grid[offset(col, row)] = value;
	}

	char get(size_t col, size_t row) {
		return grid[offset(col, row)];
	}

private:
	char* grid = nullptr;
	size_t num_cols = 0, num_rows = 0;

	bool valid(size_t col, size_t row) {
		return ((col >= 0) && (col < num_cols) && (row >= 0) && (row < num_rows));
	}

	void setup(const vector<string>& lines);

	// Locate the part numbers in the grid and collect them in a vector.
	vector<Part> scan_for_parts();

	// Locate the positions of the gears in the grid.
	map<int, Gear> scan_for_gears();

	Part get_part_at(size_t c, size_t r);
	int find_part(const vector<Part>& parts, size_t col, size_t row);
	bool adjacent_to_symbol(size_t c, size_t r);
};
