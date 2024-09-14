#include "Day14.h"
#include "common.h"
#include <iostream>

void Day14::execute()
{
    std::cout << "Part 1. Test Input..." << std::endl;
    part1("input/test14.txt");
    std::cout << "Part 1. Puzzle Input..." << std::endl;
    part1("input/input14.txt");
}

void Day14::part1(const char* fileName)
{
    auto lines = read_input_file(fileName);
    process_lines(lines);
    roll_north();
    int total = calculate_load();
    std::cout << "Total load is " << total << endl;
}

void Day14::part2(const char* fileName)
{
}

void Day14::process_lines(const std::vector<std::string>& lines)
{
    numRows = lines.size();
    if (numRows > 0) {
        numCols = lines[0].size();
    }
    if (grid != nullptr) {
        delete grid;
        grid = nullptr;
    }
    grid = new char[numCols * numRows];
    for (int i = 0; i < numRows; i++) {
        string s = lines[i];
        for (int j = 0; j < numCols; j++) {
            set(j, i, s[j]);
        }
    }
}

void Day14::roll_north()
{
    for (int j = 0; j < numCols; j++) {
        for (int i = 1; i < numRows; i++) {
            if (get(j, i) == 'O') {
                // Roll the rock north.
                int k = i - 1;
                while (k >= 0 && get(j, k) == '.') {
                    k--;
                }
                set(j, i, '.');
                set(j, k + 1, 'O');
            }
        }
    }
}

int Day14::calculate_load()
{
    int score = 0;
    for (int i = 0; i < numRows; i++) {
        int wt = numRows - i;
        for (int j = 0; j < numCols; j++) {
            if (get(j, i) == 'O') {
                score += wt;
            }
        }
    }
    return score;
}
