#include "Day1.h"
#include "common.h"
#include <iostream>

void Day1::execute()
{
	cout << "Part 1. Test Input..." << endl;
	part1("input/test1a.txt");
	cout << "Part 1. Actual Input..." << endl;
	part1("input/input1.txt");
	cout << "Part 2. Test Input..." << endl;
	part2("input/test1b.txt");
	cout << "Part 2. Actual Input..." << endl;
	part2("input/input1.txt");
}

void Day1::part1(const char* fileName) const {
	auto lines = read_input_file(fileName);
	int sum = 0;
	for (string line : lines) {
		string::size_type firstDigitPos, lastDigitPos;
		firstDigitPos = line.find_first_of(DIGITS);
		lastDigitPos = line.find_last_of(DIGITS);
		int value = (line[firstDigitPos] - '0') * 10 + (line[lastDigitPos] - '0');
		sum += value;
	}
	cout << "Input from file " << fileName << " - Sum is " << sum << endl;
}

void Day1::part2(const char* fileName) const {
	auto lines = read_input_file(fileName);
	int sum = 0, firstDigit = 0, lastDigit = 0, value = 0;
	for (string line : lines) {
		if (locateDigits(line, firstDigit, lastDigit)) {
			value = firstDigit * 10 + lastDigit;
//			cout << value << endl;
			sum += value;
		}
	}
	cout << "Input from file " << fileName << " - Sum is " << sum << endl;
}

bool Day1::locateDigits(const string line, int& firstDigit, int& lastDigit) const
{
	// Find the digit positions first.
	string::size_type firstPos = line.find_first_of(DIGITS);
	if (firstPos != string::npos) {
		firstDigit = line[firstPos] - '0';
	}
	string::size_type lastPos = line.find_last_of(DIGITS);
	if (lastPos != string::npos) {
		lastDigit = line[lastPos] - '0';
	}
	// Then look for the words.
	const vector<string> digitNames = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	for (int d = 0; d < 10; d++) {
		auto f = line.find(digitNames[d]);
 		if (f != string::npos) {
			if ((firstPos == string::npos) || (f < firstPos)) {
				firstPos = f;
				firstDigit = d;
			}
		}
		auto l = line.rfind(digitNames[d]);
		if (l != string::npos) {
			if ((lastPos == string::npos) || (l > lastPos)) {
				lastPos = l;
				lastDigit = d;
			}
		}
	}
	return ((firstPos != string::npos) && (lastPos != string::npos));
}
