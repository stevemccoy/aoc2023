#include "Day4.h"
#include "common.h"
#include <iostream>
#include <deque>

void Day4::execute()
{
	cout << "Part 1. Test Input..." << endl;
	part1("input/test4.txt");
	cout << "Part 1. Actual Input..." << endl;
	part1("input/input4.txt");
	cout << "Part 2. Test Input..." << endl;
	part2("input/test4.txt");
	cout << "Part 2. Actual Input..." << endl;
	part2("input/input4.txt");
}

void Day4::part1(const char* fileName)
{
	auto lines = read_input_file(fileName);
	int sum = 0;
	for (string line : lines) {
		int winCount = countMatches(line);
		int cardScore = score(winCount);
		sum += cardScore;
	}
	cout << "Input from file " << fileName << " - Total points = " << sum << endl;
}

void Day4::part2(const char* fileName)
{
	auto lines = read_input_file(fileName);
	int sum = 0;
	deque<int> multiples;
	multiples.insert(multiples.begin(), lines.size(), 1);
	int index = 0, m = 1, winCount = 1;
	string line;
	while (index < lines.size()) {
		line = lines[index];
		// Add the scratchcard counts to running total.
		m = multiples[index];
		// Count matches and update multiple counts, if we're still winning.
		winCount = countMatches(line);
		for (int i = 1; i <= winCount; i++) {
			multiples[index + i] += m;
		}
		index++;
	}
	// Add up the numbers of cards.
	for (int i = 0; i < multiples.size(); i++) {
		sum += multiples[i];
	}
	cout << "Input from file " << fileName << " - Total cards = " << sum << endl;
}

int Day4::countMatches(string line) {
	auto split0 = split_delim(line, ':');
	auto split1 = split_delim(split0[1], '|');
	auto winners = extract_numbers(split1[0]);
	auto myNumbers = extract_numbers(split1[1]);
	int winCount = 0;
	for (auto n : myNumbers) {
		for (auto w : winners) {
			if (n == w) {
				winCount++;
			}
		}
	}
	return winCount;
}

vector<int> Day4::extract_numbers(string s) {
	vector<int> result;
	int n = 0;
	for (auto& ns : split_delim(s, ' ')) {
		if (sscanf_s(ns.c_str(), "%d", &n) > 0) {
			result.push_back(n);
		}
	}
	return result;
}

int Day4::score(unsigned int numMatches) {
	if (numMatches < 3) {
		return numMatches;
	}
	int s = 1;
	for (int i = numMatches - 1; i; i--) {
		s *= 2;
	}
	return s;
}
