#include "Day15.h"
#include "common.h"
#include <iostream>

void Day15::execute()
{
	std::cout << "Part 1. Test Input..." << std::endl;
	part1("input/test15.txt");
	std::cout << "Part 1. Puzzle Input..." << std::endl;
	part1("input/input15.txt");
	std::cout << "Part 2. Test Input..." << std::endl;
	part2("input/test15.txt");
	std::cout << "Part 2. Puzzle Input..." << std::endl;
	part2("input/input15.txt");
}

void Day15::part1(const char* fileName)
{
	int total = 0;
	int h = 0;
	auto lines = read_input_file(fileName);
	for (string line : lines) {
		auto terms = split_delim(line, ',');
		for (auto term : terms) {
			h = hash(term);
			total += h;
			// std::cout << term << " becomes " << h << endl;
		}
	}
	std::cout << "Total of hashes = " << total << endl;
}

void Day15::part2(const char* fileName)
{
	std::vector<Lens> box[256];
	for (auto v : box) {
		v.clear();
	}
	auto lines = read_input_file(fileName);
	for (string line : lines) {
		auto terms = split_delim(line, ',');
		for (auto term : terms) {
			int index = term.find_first_of("-=");
			string lab = term.substr(0, index);
			char operation = term[index];
			string value = term.substr(index + 1, term.length() - lab.length());
			int location = hash(lab);

			switch (operation) {
			case '-':
				remove_lens(box[location], lab);
				break;
			case '=':
				assign_lens(box[location], lab, value);
				break;
			}
		}
	}
	int sum = 0;
	for (int bi = 0; bi < 256; bi++) {
		for (int i = 0; i < box[bi].size(); i++) {
			sum += (i + 1) * (bi + 1) * (box[bi][i].second);
		}
	}
	cout << "Answer is " << sum << endl;
}

int Day15::hash(string s)
{
	int n = s.size();
	const char* cp = s.c_str();
	int current = 0;
	for (int i = 0; i < n; i++) {
		current = ((current + cp[i]) * 17) % 256;
	}
	return current;
}

void Day15::remove_lens(vector<Lens>& box, const string& label)
{
	for (auto i = box.begin(); i != box.end(); i++) {
		if (i->first == label) {
			box.erase(i);
			return;
		}
	}
	return;
}

void Day15::assign_lens(std::vector<Lens>& box, const std::string& label, const std::string& value)
{
	int num = value[0] - '0';
	for (auto i = box.begin(); i != box.end(); i++) {
		if (i->first == label) {
			i->second = num;
			return;
		}
	}
	box.emplace_back(label, num);
	return;
}
