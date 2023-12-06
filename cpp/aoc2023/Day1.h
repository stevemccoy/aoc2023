#pragma once
#include "Problem.h"
class Day1 : public Problem
{
public:
	Day1() {
		init("Day 1. Trebuchet?!", "Trebuchet?!");
	}
	void execute();
	void part1(const char* fileName) const override;
	void part2(const char* fileName) const override;
private:
	const char* DIGITS = "0123456789";

	// Find the first and last digits (as digit chars or number words) in the given line.
	bool locateDigits(const string line, int& firstDigit, int& lastDigit) const;
};
