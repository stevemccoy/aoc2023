#include "Day13.h"
#include "Problem.h"
#include "common.h"

#include <iostream>

void Day13::execute()
{
    std::cout << "Part 1. Test Input..." << endl;
    part1("input/test13.txt");
    std::cout << "Part 1. Puzzle Input..." << endl;
    part1("input/input13.txt");
    std::cout << "Part 2. Test Input..." << endl;
    part2("input/test13.txt");
    //std::cout << "Part 2. Puzzle Input..." << endl;
    //part2("input/input10.txt");
}

// 20527 too low.
void Day13::part1(const char* fileName)
{
    auto lines = read_input_file(fileName);

    // Add up the number of columns to left of vertical line of reflection;
    // Also add 100 x number of rows above each horizontal line of reflection;
    // Sum of these two totals is the summary of the pattern notes.
    int pscore = 0, total = 0;
    while (process_frame(lines, pscore, 0)) {
        cout << "Score " << pscore << endl;
        total += pscore;
    }
    std::cout << "Total of pattern scores is " << total << endl;
}

void Day13::part2(const char* fileName)
{
    auto lines = read_input_file(fileName);

    // Same as above, just allowing exactly one smudge to break the perfect reflection.
    int pscore = 0, total = 0;
    while (process_frame(lines, pscore, 1)) {
        cout << "Score " << pscore << endl;
        total += pscore;
    }
    std::cout << "Total of pattern scores is " << total << endl;
}

bool Day13::process_frame(std::vector<std::string>& lines, int& score, int smudges) {
    // Grab frame first.
    vector<string> f;
    while (!lines.empty() && lines[0].empty()) {
        lines.erase(lines.begin());
    }
    while (!lines.empty() && !lines[0].empty()) {
        f.push_back(lines[0]);
        lines.erase(lines.begin());
    }
    // Find vertical axis of reflection.
    int nr = f.size();
    if (nr < 1) {
        return false;
    }
    int nc = f[0].size();
    int count = 0;
    for (int c = 1; c < nc; c++) {
        bool found = true;
        count = 0;
        for (auto s : f) {
            if (!found) {
                break;
            }
            for (int i = c - 1, j = c; i >= 0 && j < nc; i--, j++) {
                if (s[i] != s[j]) {
                    if (++count > smudges) {
                        found = false;
                        break;
                    }
                }
            }
        }
        if (found && (count == smudges)) {
            score = c;
            return true;
        }
    }

    // Find horizontal axis of reflection.
    for (int r = 1; r < nr; r++) {
        bool found = true;
        count = 0;
        for (int i = r - 1, j = r; i >= 0 && j < nr; i--, j++) {
            if (f[i] != f[j]) {
                int diffs = compare_rows(f[i], f[j]);
                if (diffs < 0) {
                    cerr << "Error - badly formed rows." << endl;
                    return false;
                }
                count += diffs;
                if (count > smudges) {
                    found = false;
                    break;
                }
            }
        }
        if (found && (count == smudges)) {
            score = 100 * r;
            return true;
        }
    }
    score = 0;
    return false;
}

int Day13::compare_rows(string row1, string row2)
{
    int count = 0;
    if (row1.size() == row2.size()) {
        for (int i = 0; i < row1.size(); i++) {
            if (row1[i] != row2[i]) {
                count++;
            }
        }
        return count;
    }
    return -1;
}
