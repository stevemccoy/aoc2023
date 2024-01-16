#pragma once
#include "Problem.h"
#include "Position.h"

#include <vector>
#include <map>

class Day11 : public Problem
{
public:
	Day11() {
		init("Day 11", "Cosmic Expansion");
	}
	void execute();
	void part1(const char* fileName);
	void part2(const char* fileName) { part2(fileName, 2); }

	void part2(const char* fileName, int factor);
	void solve(const char* fileName, int factor);

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

	void row_counts(vector<int>& counts);
	void col_counts(vector<int>& counts);
	number distance_sum(const vector<number>& distances) const;

	void setup(const vector<string>& lines);

	void scan_for_galaxies(vector<PosPtr>& galaxies);
	void expand(vector<PosPtr>& galaxies, int factor) ;

	vector<number> calculate_distances(const vector<PosPtr>& positions) const {
		vector<number> distances;
		for (int g1 = 0; g1 < positions.size(); g1++) {
			for (int g2 = 0; g2 < positions.size(); g2++) {
				if (g1 > g2) {
					distances.push_back(positions[g1]->distance(*positions[g2].get()));
				}
			}
		}
		return distances;
	}
};
