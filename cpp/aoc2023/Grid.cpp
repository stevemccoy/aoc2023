#include "Grid.h"

void Grid::clear()
{
	m_grid.clear();
	num_cols = num_rows = 0;
}

int Grid::get(int c, int r)
{
	auto i = m_grid.find(Position(c, r));
	if (i == m_grid.end()) {
		return m_default;
	}
	return 0;
}

void Grid::set(int c, int r, int v)
{
	m_grid[Position(c, r)] = v;
}

