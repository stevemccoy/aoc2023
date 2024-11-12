#include "Day18.h"
#include "common.h"
#include <iostream>
#include <set>
#include "Position.h"

using namespace std;

void Day18::execute()
{
    std::cout << "Part 1. Test Input..." << endl;
    part1("input/test18.txt");
    //std::cout << "Part 1. Puzzle Input..." << endl;
    //part1("input/input10.txt");
    //std::cout << "Part 2. Test Input..." << endl;
    //part2("input/test10c.txt");
    //std::cout << "Part 2. Puzzle Input..." << endl;
    //part2("input/input10.txt");
}

void Day18::part1(const char* fileName)
{
    auto lines = read_input_file(fileName);
    char direction;
    int distance;
    char colourCode[8];
    int c = 0, r = 0;
    int dc = 0, dr = 0;
    Position topLeft, bottomRight;
    for (string line : lines) {
        int n = sscanf_s(line.c_str(), "%c %d (#%s)", &direction, 1, &distance, colourCode, 8);
        if ((n == EOF) || (n == 0)) {
            break;
        }
        switch (direction) {
        case 'U':   dr = -1; dc = 0; break;
        case 'R':   dr = 0; dc = 1; break;
        case 'D':   dr = 1; dc = 0; break;
        case 'L':   dr = 0; dc = -1; break;
        default:    cout << "ERROR in line: " << line << endl;
            return;
        }
        while (distance > 0) {
            c += dc;
            r += dr;
            distance--;
        }
        extend_bounding_box(topLeft, bottomRight, c, r);
    }
    //setup(lines);
    //Path path;
    //extract_path(path);
    //size_t count = path.size() / 2;
    size_t count = 0;
    std::cout << "Path extends to " << count << " farthest from the start." << endl;
}

void Day18::part2(const char* fileName)
{
    auto lines = read_input_file(fileName);
    //setup(lines);
    //display();
    //Path path, leftSide;
    //extract_path(path);
    //mark_up(path, 'p');
    //left_side2(path, leftSide);
    //mark_up(leftSide, '*');
    //display();
    //spread(leftSide, path);
    //mark_up(leftSide, '*');
    //display();
}

void Day18::extend_bounding_box(Position& topLeft, Position& bottomRight, int col, int row)
{
    if (col < topLeft.col) {
        topLeft.col = col;
    }
    else if (col > bottomRight.col) {
        bottomRight.col = col;
    }
    if (row < topLeft.row) {
        topLeft.row = row;
    }
    else if (row > bottomRight.row) {
        bottomRight.row = row;
    }
}

void Day18::setup(const vector<string>& lines) {
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
