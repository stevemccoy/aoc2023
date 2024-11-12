#pragma once
#include "Problem.h"
#include "Position.h"

#include <vector>
#include <map>
#include <set>
#include <deque>

// Type to use for tracking paths through the grid.
typedef map<Position, int> Path;

class Day18 : public Problem
{
public:
	Day18() {
		init("Day 18", "Lavaduct Lagoon");
	}

	// Inherited via Problem
	virtual void execute() override;
	virtual void part1(const char* fileName) override;
	virtual void part2(const char* fileName) override;

	size_t offset(size_t col, size_t row) const {
		return row * num_cols + col;
	}

	void set(size_t col, size_t row, char value) {
		grid[offset(col, row)] = value;
	}

	char get(size_t col, size_t row) const {
		return grid[offset(col, row)];
	}

	string row_chars(size_t r) const {
		string s;
		s.assign(grid + r * num_cols, num_cols);
		return s;
	}

private:
	char* grid = nullptr;
	size_t num_cols = 0, num_rows = 0;

	//void setup(const vector<string>& lines);

	bool valid(int col, int row) const {
		return ((col >= 0) && (col < num_cols) && (row >= 0) && (row < num_rows));
	}

	void extend_bounding_box(Position& topLeft, Position& bottomRight, int col, int row);
};
