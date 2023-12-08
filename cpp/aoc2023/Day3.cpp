#include "Day3.h"
#include "common.h"
#include <iostream>
#include <set>

// static int nextGearId = 1;

void Day3::execute()
{
	cout << "Part 1. Test Input..." << endl;
	part1("input/test3.txt");
	cout << "Part 1. Actual Input..." << endl;
	part1("input/input3.txt");
	cout << "Part 2. Test Input..." << endl;
	part2("input/test3.txt");
	cout << "Part 2. Actual Input..." << endl;
	part2("input/input3.txt");
}

void Day3::part1(const char* fileName)
{
    auto lines = read_input_file(fileName);
    setup(lines);
    vector<Part> parts = scan_for_parts();
    cout << "Detected Part Numbers..." << endl;
    int sum = 0;
    for (auto& p : parts) {
        sum += p.value ;
    }
    cout << "Sum of all given part numbers = " << sum << endl;
}

void Day3::part2(const char* fileName)
{
    auto lines = read_input_file(fileName);
    setup(lines);
    vector<Part> parts = scan_for_parts();
    map<int, Gear> gears = scan_for_gears();
    // Look for the gears and find those that have two part numbers next door.
    std::set<int> neighbours;
    int id = -1;
    int sum = 0;
    for (auto p : gears) {
        auto g = p.second;
        neighbours.clear();
        for (size_t r = (g.row > 0 ? g.row - 1 : 0); ((r < num_rows) && (r <= g.row + 1)); r++) {
            for (size_t c = (g.col > 0 ? g.col - 1 : 0); ((c < num_cols) && (c <= g.col + 1)); c++) {
                id = find_part(parts, c, r);
                if (id != -1) {
                    neighbours.insert(id);
                }
            }
        }
        // If parts mesh, do thing with gear ratios, etc.        
        if (neighbours.size() == 2) {
            int ratio = 1;
            for (auto pid : neighbours) {
                ratio *= parts[pid].value;
            }
            sum += ratio;
        }
    }
    cout << "Sum of all given gear ratios = " << sum << endl;
}

int Day3::find_part(const vector<Part>& parts, size_t col, size_t row) {
    int i = 0;
    for (auto& p : parts) {
        if ((row == p.row) && (col >= p.startCol) && (col <= p.endCol)) {
            return i;
        }
        i++;
    }
    return (i == parts.size() ? -1 : i);
}

void Day3::setup(const vector<string>& lines) {
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

vector<Part> Day3::scan_for_parts()
{
    vector<Part> result;
    int n = 0;
    char ch = ' ';
    for (size_t r = 0; r < num_rows; r++) {
        for (size_t c = 0; c < num_cols; c++) {
            ch = get(c, r);
            // Find the first digit in a possible part number.
            if (isdigit(ch)) {
                Part p = get_part_at(c, r);
                if (adjacent_to_symbol(c, r)) {
                    result.emplace_back(p);
                }
                // Skip the characters just consumed.
                while ((c < num_cols) && isdigit(get(c, r))) {
                    c++;
                }
            }
        }
    }
    return result;
}

map<int, Gear> Day3::scan_for_gears() {
    map<int, Gear> result;
    char ch;
    for (size_t r = 0; r < num_rows; r++) {
        for (size_t c = 0; c < num_cols; c++) {
            ch = get(c, r);
            // Find the first digit in a possible part number.
            if (ch == '*') {
                Gear g(c, r);
                result.insert(pair<int, Gear>(g.id, g));
            }
        }
    }
    return result;
}

bool Day3::adjacent_to_symbol(size_t cs, size_t rs)
{
    size_t ce = cs;
    for (size_t c = cs; ((c >= 0) && (c < num_cols) && (isdigit(get(c, rs)))); c++) {
        if ((rs > 0) && (get(c, rs - 1) != '.')) {
            return true;
        }
        if ((rs < num_rows - 1) && (get(c, rs + 1) != '.')) {
            return true;
        }
        ce = c;
    }
    for (size_t r = (rs > 0 ? rs - 1 : 0); r <= rs + 1; r++) {
        if ((r >= 0) && (r < num_rows)) {
            if ((cs > 0) && (get(cs - 1, r) != '.')) {
                return true;
            }
            if ((ce < num_cols - 1) && (get(ce + 1, r) != '.')) {
                return true;
            }
        }
    }
    return false;
}

Part Day3::get_part_at(size_t c, size_t r) {
    char buffer[80];
    size_t count = 0;
    size_t cs = c;
    while (isdigit(get(c, r))) {
        count++;
        c++;
    }
    strncpy_s(buffer, (const char*)(grid + offset(cs, r)), count);
    buffer[count] = '\0';
    Part p(atoi(buffer), r, cs, c - 1);
    return p;
}
