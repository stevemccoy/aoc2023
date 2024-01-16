#pragma once
#include <cstdlib>

#include <memory>

typedef int64_t number;

// Single location on the grid.
class Position {
public:
	number col, row;
	Position() : col(0), row(0) {}
	Position(number c, number r) : col(c), row(r) {}
	Position(const Position& other) : col(other.col), row(other.row) {}
	virtual ~Position() {}
	bool operator==(const Position& other) const {
		return ((col == other.col) && (row == other.row));
	}
	bool operator!=(const Position& other) const {
		return !(this->operator==(other));
	}
	bool operator<(const Position& other) const {
		return (row == other.row) ? (col < other.col) : (row < other.row);
	}
	void move(char direction) {
		switch (direction)
		{
		case 'N': row--; break;
		case 'S': row++; break;
		case 'E': col++; break;
		case 'W': col--; break;
		default: break;
		}
	}
	number distance(const Position& other) const {
		return (labs(col - other.col) + labs(row - other.row));
	}
};

typedef std::shared_ptr<Position> PosPtr;
