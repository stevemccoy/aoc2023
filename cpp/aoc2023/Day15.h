#pragma once
#include "Problem.h"
#include<vector>

// Name and focal length of a lens.
typedef std::pair<string, int> Lens;
using namespace std;


class Day15 : public Problem
{
public:
	Day15() {
		init("Day 15", "Lens Library");
	}
	void execute();
	void part1(const char* fileName);
	void part2(const char* fileName);

	int hash(string s);

private:
	void remove_lens(vector<Lens>& box, const string& label);
	void assign_lens(vector<Lens>& box, const string& label, const string& value);
};

