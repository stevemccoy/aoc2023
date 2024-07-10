#include "Day10.h"
#include "common.h"
#include <iostream>
#include <set>

using namespace std;

void Day10::execute()
{
    //std::cout << "Part 1. Test Input..." << endl;
    //part1("input/test10.txt");
    //std::cout << "Part 1. Puzzle Input..." << endl;
    //part1("input/input10.txt");
    std::cout << "Part 2. Test Input..." << endl;
    part2("input/test10c.txt");
    std::cout << "Part 2. Puzzle Input..." << endl;
    part2("input/input10.txt");
}

void Day10::part1(const char* fileName)
{
    auto lines = read_input_file(fileName);
    setup(lines);
    Path path;
    extract_path(path);
    size_t count = path.size() / 2;
    std::cout << "Path extends to " << count << " farthest from the start." << endl;
}

void Day10::part2(const char* fileName)
{
    auto lines = read_input_file(fileName);
    setup(lines);
    display();
    Path path, leftSide;
    extract_path(path);
    mark_up(path, 'p');
    left_side2(path, leftSide);
    mark_up(leftSide, '*');
    display();
    spread(leftSide, path);
    mark_up(leftSide, '*');
    display();
}

void Day10::extract_path(Path& path) {
    path.clear();
    Position p1f, p2f, p1v, p2v, p1t, p2t;
    if (findStart(p1f)) {
        path[p1f] = 0;
        p2f = p1f;
        auto starts = neighbours(p1f);
        p1v = starts[0], p2v = starts[1];
        path[p1v] = 1, path[p2v] = -1;
        bool done = false;
        int count = 1;
        while (!done) {
            extend(p1f, p1v, p1t);
            extend(p2f, p2v, p2t);
            count++;
            if (p1t == p2t) {
                done = true;
                path[p1t] = count;
            }
            else if (p1v == p2t) {
                done = true;
            }
            else {
                p1f = p1v, p1v = p1t;
                path[p1t] = count;
                p2f = p2v, p2v = p2t;
                path[p2t] = -count;
            }
        }
    }
}

bool Day10::findStart(Position& pos)
{
    for (int c = 0; c < num_cols; c++) {
        for (int r = 0; r < num_rows; r++) {
            if (get(c, r) == 'S') {
                pos.col = c;
                pos.row = r;
                return true;
            }
        }
    }
    return false;
}

vector<Position> Day10::neighbours(const Position& p) const {
    vector<Position> result;
    if (can_go(p, 0, -1)) {
        result.emplace_back(p.col, p.row - 1);
    }
    if (can_go(p, 0, 1)) {
        result.emplace_back(p.col, p.row + 1);
    }
    if (can_go(p, -1, 0)) {
        result.emplace_back(p.col - 1, p.row);
    }
    if (can_go(p, 1, 0)) {
        result.emplace_back(p.col + 1, p.row);
    }
    return result;
}

bool Day10::can_go(const Position& from, int dc, int dr) const {
    if ((dc != 0) && (dr != 0)) {
        return false;
    }
    if ((dc < -1) || (dc > 1) || (dr < -1) || (dr > 1)) {
        return false;
    }
    if (valid(from.col + dc, from.row + dr)) {
        auto c = from.col + dc, r = from.row + dr;
        char ch = get(c, r);
        switch (ch) {
        case '|':
            return (dc == 0);
        case '-':
            return (dr == 0);
        case 'L':
            return ((dc == -1) || (dr == 1));
        case 'J':
            return ((dc == 1) || (dr == 1));
        case '7':
            return ((dc == 1) || (dr == -1));
        case 'F':
            return ((dc == -1) || (dr == -1));
        default:
            return false;
        }
    }
    return false;
}

bool Day10::extend(const Position& from, const Position& via, Position& to) const {
    Position next[2];
    char ch = get(via.col, via.row);
    switch (ch) {
    case '|':
        next[0] = Position(via.col, via.row + 1);
        next[1] = Position(via.col, via.row - 1);
        break;
    case '-':
        next[0] = Position(via.col + 1, via.row);
        next[1] = Position(via.col - 1, via.row);
        break;
    case 'L':
        next[0] = Position(via.col, via.row - 1);
        next[1] = Position(via.col + 1, via.row);
        break;
    case 'J':
        next[0] = Position(via.col, via.row - 1);
        next[1] = Position(via.col - 1, via.row);
        break;
    case '7':
        next[0] = Position(via.col, via.row + 1);
        next[1] = Position(via.col - 1, via.row);
        break;
    case 'F':
        next[0] = Position(via.col, via.row + 1);
        next[1] = Position(via.col + 1, via.row);
        break;
    default:
        return false;
    }
    for (int i = 0; i < 2; i++) {
        auto c = next[i].col, r = next[i].row;
        if (valid(c, r) && ((c != from.col) || (r != from.row))) {
            to.col = c;
            to.row = r;
            return true;
        }
    }
    return false;
}

void Day10::left_side(const Path& path, Path& result) {
    // All the valid positions on the left bank of the path.
    result.clear();
    // Prime the tracking from cyclical path - both end points. And invert the map given! 
    int minIndex = 0, maxIndex = 0;
    map<int, Position> posFromIndex;
    for (auto& p : path) {
        posFromIndex[p.second] = p.first;
        if (minIndex > p.second) {
            minIndex = p.second;
        }
        else if (maxIndex < p.second) {
            maxIndex = p.second;
        }
    }
    Position p1 = posFromIndex[maxIndex - 1];
    Position p2 = posFromIndex[maxIndex];
    Position p3 = posFromIndex[minIndex];
    Position t1 = left_target(p1, p2);
    Position t2 = t1;
    string shape;
    char direction;
    for (int i = minIndex + 1; i <= maxIndex; i++) {
        p1 = p2;
        p2 = p3;
        p3 = posFromIndex[i];
        t1 = t2;
        t2 = left_target(p2, p3);
        // Don't move the target on if it would be on the path.
        if (path.find(t2) != path.end()) {
            t2 = t1;
        }
        shape = this->shape(p1, p2, p3);
        for (int j = 0; j <= 1; j++) {
            direction = shape[j];
            if ((direction == 'N') || (direction == 'S')) {
                while ((t1.row != t2.row) && !off_margin(t1)) {
                    t1.move(direction);
                    if (valid(t1.col, t1.row) && (path.find(t1) == path.end()) && !isInRegion(t1, result)) {
                        result[t1] = i;
                    }
                }
            }
            else {
                while ((t1.col != t2.col) && !off_margin(t1)) {
                    t1.move(direction);
                    if (valid(t1.col, t1.row) && (path.find(t1) == path.end()) && !isInRegion(t1, result)) {
                        result[t1] = i;
                    }
                }
            }
        }
    }
}

void Day10::left_side2(const Path& path, Path& result) {
    // All the valid positions on the left bank of the path.
    result.clear();
    // Prime the tracking from cyclical path - both end points. And invert the map given! 
    int minIndex = 0, maxIndex = 0;
    map<int, Position> posFromIndex;
    for (auto& p : path) {
        posFromIndex[p.second] = p.first;
        if (minIndex > p.second) {
            minIndex = p.second;
        }
        else if (maxIndex < p.second) {
            maxIndex = p.second;
        }
    }
    Position p1 = posFromIndex[maxIndex];
    Position p2 = posFromIndex[minIndex];
    Position p3 = posFromIndex[minIndex + 1];
    Position px;
    string shape;
    char direction;
    int npi = 1;
    for (int i = minIndex; i <= maxIndex - 2; i++) {
        p1 = posFromIndex[i];
        p2 = posFromIndex[i + 1];
        p3 = posFromIndex[i + 2];
        shape = this->shape(p1, p2, p3);
        if (shape == "NN") {
            p2.neighbour('W', px);
            result[px] = npi++;
        }
        else if (shape == "SS") {
            p2.neighbour('E', px);
            result[px] = npi++;
        }
        else if (shape == "EE") {
            p2.neighbour('N', px);
            result[px] = npi++;
        }
        else if (shape == "WW") {
            p2.neighbour('S', px);
            result[px] = npi++;
        }

        else if (shape == "NE") {
            p2.neighbour('W', px);
            result[px] = npi++;
            px.row--;
            result[px] = npi++;
            px.col++;
            result[px] = npi++;
        }
        else if (shape == "NW") {
        }

        else if (shape == "SW") {
            p2.neighbour('E', px);
            result[px] = npi++;
            px.row++;
            result[px] = npi++;
            px.col--;
            result[px] = npi++;
        }
        else if (shape == "SE") {
        }

        else if (shape == "ES") {
            p2.neighbour('N', px);
            result[px] = npi++;
            px.col++;
            result[px] = npi++;
            px.row++;
            result[px] = npi++;
        }
        else if (shape == "EN") {
        }

        else if (shape == "WN") {
            p2.neighbour('S', px);
            result[px] = npi++;
            px.col--;
            result[px] = npi++;
            px.row--;
            result[px] = npi++;
        }
        else if (shape == "WS") {
        }

    }

    // Remove those elements from result that are on the path.
//    for (std::pair<Position, int>& p : result) {

//    }
}

Position Day10::left_target(const Position& p1, const Position& p2) {
    int dc = p2.col - p1.col;
    int dr = p2.row - p1.row;
    Position target;
    switch (dc) {
    case -1:    // West.
        return Position(p2.col, p2.row + 1);
    case 0: // North / South.
        return (dr == -1) ? Position(p2.col - 1, p2.row) : Position(p2.col + 1, p2.row);
    case 1:     // East.
        return Position(p2.col, p2.row - 1);
    default:
        std::cout << "Error in positions along path." << endl;
        return Position();
    }
}

string Day10::shape(const Position& p1, const Position& p2, const Position& p3) {
    char buffer[10];
    sprintf_s(buffer, "%c%c", direction(p1, p2), direction(p2, p3));
    return buffer;
}

char Day10::direction(const Position& p1, const Position& p2) {
    auto dc = p2.col - p1.col;
    auto dr = p2.row - p1.row;
    Position target;
    switch (dc) {
    case -1:    // West.
        return 'W';
    case 0: // North / South.
        return (dr == -1) ? 'N' : 'S';
    case 1:     // East.
        return 'E';
    default:
        std::cout << "Error in positions along path." << endl;
        return 'X';
    }
}

bool Day10::isInRegion(const Position& p, const Path& region) const {
    return (region.find(p) != region.end());
}

void Day10::spread(Path& region, const Path& path) {
    deque<std::pair<Position, int>> open;
    vector<Position> nb;
    // Prime an open list with the elements known to be in the region.
    for (auto i = region.begin(); i != region.end(); i++) {
        open.emplace_back(*i);
    }
    // Work out from the open list points.
    while (open.size() > 0) {
        auto s = open.front();
        open.pop_front();
        if (!isInRegion(s.first, region)) {
            region[s.first] = s.second;
        }
        adjacent_cells(s.first, nb);
        for (auto n : nb) {
            if ((path.find(n) == path.end()) && !isInRegion(n, region)) {
                open.emplace_back(std::pair<Position, int>(n, s.second));
            }
        }
    }
    // Report the resulting cell counts.
    size_t count = num_rows * num_cols - path.size() - region.size();
    std::cout << "Path has " << path.size() << " cells in it." << endl;
    std::cout << "Expanded region has " << region.size() << " cells in it." << endl;
    std::cout << "Number of other cells = " << count << endl;
}

void Day10::adjacent_cells(const Position& p, vector<Position>& result) {
    result.clear();
    if (valid(p.col, p.row - 1)) {
        result.emplace_back(p.col, p.row - 1);
    }
    if (valid(p.col, p.row + 1)) {
        result.emplace_back(p.col, p.row + 1);
    }
    if (valid(p.col - 1, p.row)) {
        result.emplace_back(p.col - 1, p.row);
    }
    if (valid(p.col + 1, p.row)) {
        result.emplace_back(p.col + 1, p.row);
    }
}

void Day10::display() {
    std::cout << endl;
    Position p(0, 0);
    for (size_t r = 0; r < num_rows; r++) {
        std::cout << row_chars(r) << endl;
    }
    std::cout << endl;
}

void Day10::mark_up(const Path& path, char marker) {
    for (auto& pr : path) {
        Position p = pr.first;
        set(p.col, p.row, marker);
    }
}

deque<Position> Day10::edge_starts(const Path& path) const
{
    deque<Position> edges;
    Position p(0, 0);
    // Top row.
    for (int c = 0; c < num_cols; c++) {
        p.col = c;
        // Don't bother if already on the path.
        if (path.find(p) == path.end()) {
            edges.emplace_back(p);
        }
    }
    // Bottom row.
    p.row = num_rows - 1;
    for (int c = 0; c < num_cols; c++) {
        p.col = c;
        // Don't bother if already on the path.
        if (path.find(p) == path.end()) {
            edges.emplace_back(p);
        }
    }
    // Left column.
    p.col = 0;
    for (int r = 1; r < num_rows - 1; r++) {
        p.row = r;
        // Don't bother if already on the path.
        if (path.find(p) == path.end()) {
            edges.emplace_back(p);
        }
    }
    // Right column.
    p.col = num_cols - 1;
    for (int r = 1; r < num_rows - 1; r++) {
        p.row = r;
        // Don't bother if already on the path.
        if (path.find(p) == path.end()) {
            edges.emplace_back(p);
        }
    }
    return edges;
}

void Day10::setup(const vector<string>& lines) {
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
