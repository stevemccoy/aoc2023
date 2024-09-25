#pragma once
#include "Problem.h"
#include<vector>
#include<deque>
#include<unordered_map>
#include<set>
#include<map>

using namespace std;

typedef enum { n, e, w, s } Direction;
typedef struct { int c; int r; } Location;
typedef struct {
	Location loc; 
	Direction hdg; 
} Beam;

class Day16 : public Problem
{
public:
	Day16() {
		grid = nullptr;
		numCols = numRows = 0;
		init("Day 16", "The Floor Will Be Lava");
	}
	void execute();
	void part1(const char* fileName);
	size_t count_locations(const std::map<Beam, bool>& seen);
	void part2(const char* fileName);

private:
	size_t numRows;
	size_t numCols;
	char* grid;

	char get(int c, int r) { return grid[c + r * numCols]; }
	void set(int c, int r, char v) { grid[c + r * numCols] = v; }

	void process_lines(const vector<string>& lines);
	void project(deque<Beam>& open, std::map<Beam, bool>& seen);
	void reflect(char mirror, Beam b, deque<Beam>& open);

	void split(char splitter, Beam b, deque<Beam>& open);

	void coast(Beam b, deque<Beam>& open);

	bool step(Beam& b);
	bool isBeamValid(const Beam& beam) {
		return (beam.loc.c >= 0) && (beam.loc.c < numCols) && (beam.loc.r >= 0) && (beam.loc.r < numRows);
	}
};
