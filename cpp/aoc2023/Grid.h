#pragma once
#include "Position.h"
#include <map>

class Grid
{
public:
	Grid(int default_value) { m_default = default_value; }
	void clear();

	int get(int c, int r);
	void set(int c, int r, int v);

private:
	int m_default = 0;
	int num_rows = 0;
	int num_cols = 0;
	std::map<Position, int> m_grid;

};
