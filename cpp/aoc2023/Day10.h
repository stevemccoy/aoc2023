#pragma once
#include "Problem.h"
#include "Position.h"

#include <vector>
#include <map>
#include <set>
#include <deque>

// Type to use for tracking paths through the grid.
typedef map<Position, int> Path;

class Day10 : public Problem
{
public:
	Day10() {
		init("Day 10", "Pipe Maze");
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

	void setup(const vector<string>& lines);

	bool valid(int col, int row) const {
		return ((col >= 0) && (col < num_cols) && (row >= 0) && (row < num_rows));
	}

	void extract_path(Path& path);

	bool findStart(Position& pos);

	vector<Position> neighbours(const Position& p) const;
	bool can_go(const Position& from, int dc, int dr) const;

	bool extend(const Position& from, const Position& via, Position& to) const;

	void left_side(const Path& path, Path& result);
	Position left_target(const Position& p1, const Position& p2);
	string shape(const Position& p1, const Position& p2, const Position& p3);
	char direction(const Position& p1, const Position& p2);
	bool isInRegion(const Position& p, const Path& region) const;

	bool off_margin(const Position& p) const {
		return ((p.col < -1) || (p.col > num_cols) || (p.row < -1) || (p.row > num_cols));
	}

	void spread(Path& openList, const Path& path);

	void display();
	void mark_up(const Path& path, char marker);

	deque<Position> edge_starts(const Path& path) const;
	void adjacent_cells(const Position& p, vector<Position>& result);
};
