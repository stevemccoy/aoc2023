#include "Day11.h"
#include "common.h"
#include <iostream>

void Day11::execute()
{
	cout << "Part 1. Test Input..." << endl;
	part1("input/test11.txt");
	cout << "Part 1. Actual Input..." << endl;
	part1("input/input11.txt");
	cout << "Part 2. Test Input..." << endl;
	part2("input/test11.txt", 100);
	cout << "Part 2. Actual Input..." << endl;
	part2("input/input11.txt", 1000000L);
}

void Day11::part1(const char* fileName)
{
    cout << "Part 1." << endl;
    solve(fileName, 2);
}

void Day11::part2(const char* fileName, int factor)
{
    cout << "Part 2." << endl;
    solve(fileName, factor);
}

void Day11::solve(const char* fileName, int factor)
{
    auto lines = read_input_file(fileName);
    setup(lines);
    vector<PosPtr> galaxies;
    scan_for_galaxies(galaxies);
    expand(galaxies, factor);
    vector<number> distances = calculate_distances(galaxies);
    number result = distance_sum(distances);
    cout << "Sum of all galaxy distances = " << result << endl;
}

void Day11::setup(const vector<string>& lines) {
    num_rows = lines.size();
    num_cols = lines[0].size();
    if (grid != nullptr) {
        delete[] grid;
        grid = nullptr;
    }

    size_t num_chars = num_cols * num_rows * sizeof(char);
    grid = new char[num_chars];
    memset(grid, 0, num_chars);

    int r = 0, c = 0;
    for (auto& line : lines) {
        c = 0;
        for (auto ch : line) {
            set(c++, r, ch);
        }
        r++;
    }
}

void Day11::scan_for_galaxies(vector<PosPtr>& result)
{
    result.clear();
    for (size_t r = 0; r < num_rows; r++) {
        for (size_t c = 0; c < num_cols; c++) {
            if (get(c, r) == '#') {
                PosPtr p = std::make_shared<Position>(c, r);
                result.push_back(p);
            }
        }
    }
}

void Day11::expand(vector<PosPtr>& galaxies, int factor) {
    int increment = factor - 1;
    // Expand by empty columns.
    vector<int> cc;
    col_counts(cc);
    for (int i = cc.size() - 1; i >= 0; i--) {
        if (cc[i] == 0) {
            for (auto gp : galaxies) {
                if (gp->col > i) {
                    gp->col += increment;
                }
            }
        }
    }
    // Expand by empty rows.
    vector<int> rc;
    row_counts(rc);
    for (int i = rc.size() - 1; i >= 0; i--) {
        if (rc[i] == 0) {
            for (auto gp : galaxies) {
                if (gp->row > i) {
                    gp->row += increment;
                }
            }
        }
    }
}

void Day11::col_counts(vector<int>& counts)
{
    counts.clear();
    for (int c = 0; c < num_cols; c++) {
        int count = 0;
        for (int r = 0; r < num_rows; r++) {
            if (get(c, r) == '#') {
                count++;
            }
        }
        counts.push_back(count);
    }
}

void Day11::row_counts(vector<int>& counts)
{
    counts.clear();
    for (int r = 0; r < num_rows; r++) {
        int count = 0;
        for (int c = 0; c < num_cols; c++) {
            if (get(c, r) == '#') {
                count++;
            }
        }
        counts.push_back(count);
    }
}

number Day11::distance_sum(const vector<number>& distances) const {
    number count = 0;
    for (auto& d : distances) {
        count += d;
    }
    return count;
}
