#include "Day16.h"
#include "common.h"
#include<iostream>

// Beam comparator (using lambda).
class BeamCompare
{
    bool operator()(const Beam& b1, const Beam& b2) const /* noexcept */
    {
        return (b1.loc.c < b2.loc.c) || (b1.loc.r < b2.loc.r) || (b1.hdg < b2.hdg);
    }
};

static bool operator<(const Beam& b1, const Beam& b2) /* noexcept */
{
    return (b1.loc.c < b2.loc.c) || (b1.loc.r < b2.loc.r) || (b1.hdg < b2.hdg);
}

static bool operator<(const Location& b1, const Location& b2) /* noexcept */
{
    return (b1.c < b2.c) || (b1.r < b2.r);
}


void Day16::execute()
{
	std::cout << "Part 1. Test Input..." << std::endl;
	part1("input/test16.txt");
}

void Day16::part1(const char* fileName)
{
	int total = 0;
	int h = 0;
	auto lines = read_input_file(fileName);
    process_lines(lines);
    deque<Beam> open;
    std::map<Beam, bool> seen;
    seen.clear();
    Beam start = { {0,0}, e };
    open.push_back(start);
    project(open, seen);
    size_t energised = count_locations(seen);

	std::cout << "Total energised cells = " << energised << endl;
}

size_t Day16::count_locations(const std::map<Beam, bool>& seen) {
    std::map<Location, bool> locs;
    for (auto i = seen.begin(); i != seen.end(); i++) {
        locs[i->first.loc] = true;
    }
    return locs.size();
}

void Day16::process_lines(const vector<string>& lines)
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

void Day16::part2(const char* fileName)
{
}

void Day16::project(deque<Beam>& open, std::map<Beam, bool>& seen)
{
    while (!open.empty()) {
        Beam b1 = open.front();
        open.pop_front();
        if (seen.find(b1) == seen.end()) {
            seen[b1] = true;
            char ch = get(b1.loc.c, b1.loc.r);
            switch (ch) {
            case '/': case '\\':    reflect(ch, b1, open);  break;
            case '|': case '-':     split(ch, b1, open);    break;
            case '.':               coast(b1, open);        break;
            }
        }
    }
}

void Day16::reflect(char mirror, Beam b, deque<Beam>& open) {
    if (mirror == '/') {
        switch (b.hdg) {
        case n: b.hdg = e; break;
        case e: b.hdg = n; break;
        case s: b.hdg = w; break;
        case w: b.hdg = s; break;
        }
    }
    else if (mirror == '\\') {
        switch (b.hdg) {
        case n: b.hdg = w; break;
        case e: b.hdg = s; break;
        case s: b.hdg = e; break;
        case w: b.hdg = n; break;
        }

    }
    coast(b, open);
}

void Day16::split(char splitter, Beam b, deque<Beam>& open) {
    if (splitter == '|') {
        switch (b.hdg) {
        case n: case s:
            coast(b, open);
            break;
        case e: case w:
            b.hdg = n;
            coast(b, open);
            b.hdg = s;
            coast(b, open);
            break;
        }
    }
    else if (splitter == '-') {
        switch (b.hdg) {
        case n: case s:
            b.hdg = e;
            coast(b, open);
            b.hdg = w;
            coast(b, open);
            break;
        case e: case w:
            coast(b, open);
            break;
        }
    }
}

void Day16::coast(Beam b, deque<Beam>& open) {
    if (step(b)) {
        open.push_back(b);
    }
}

bool Day16::step(Beam& beam)
{
    switch (beam.hdg) {
    case n: beam.loc.r--; break;
    case e: beam.loc.c++; break;
    case s: beam.loc.r++; break;
    case w: beam.loc.c--; break;
    }
    return isBeamValid(beam);
}
