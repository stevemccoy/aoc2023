#
# Day 22 - Sand Slabs.
#

import numpy as np
import re

# Globals.

# Canvas for the game.
num_x = num_y = num_z = 1000
grid = np.zeros((num_x, num_y, num_z), dtype = np.int16)
next_id = 0

# Read the contents of the given file.
def gulp_input(file_name):
	with open(file_name, 'r') as f:
		return list(f)

def draw_line(x1, y1, z1, x2, y2, z2):
#	print((x1,y1,z1), ' to ', (x2,y2,z2))
	count = 0
	global next_id
	next_id += 1
	if x1 == x2 and z1 == z2:
		for y in range(min(y1,y2), max(y1,y2)+1):
			if grid[x1, y, z1] != 0:
				print('Occupied cell at ', (x1, y, z1), ' in draw_line.')
				return False
			grid[x1, y, z1] = next_id
			count += 1
	elif y1 == y2 and z1 == z2:
		for x in range(min(x1,x2), max(x1,x2)+1):
			if grid[x, y1, z1] != 0:
				print('Occupied cell at ', (x, y1, z1), ' in draw_line.')
				return False
			grid[x, y1, z1] = next_id
			count += 1
	elif x1 == x2 and y1 == y2:
		for z in range(min(z1,z2), max(z1,z2)+1):
			if grid[x1, y1, z] != 0:
				print('Occupied cell at ', (x1, y1, z), ' in draw_line.')
				return False
			grid[x1, y1, z] = next_id
			count += 1
	else:
		print('Error in draw_line ', (x1, y1, z1, x2, y2, z2))
		return False
	return next_id

# Mark the positions of the bricks on the grid.
def draw_bricks(lines):
	count = 0
	pattern = r'(?P<x1>\d+),(?P<y1>\d+),(?P<z1>\d+)~(?P<x2>\d+),(?P<y2>\d+),(?P<z2>\d+)'
	for line in lines:
		d = re.match(pattern, line).groupdict()
		x1,y1,z1 = int(d['x1']),int(d['y1']),int(d['z1'])
		x2,y2,z2 = int(d['x2']),int(d['y2']),int(d['z2'])
		if draw_line(x1, y1, z1, x2, y2, z2) == False:
			print('Error detected in draw_bricks, quitting.')
			return False
		count += 1
	return count


def drop_sand_grain():
	x,y = 500,0
	if grid[y, x] != 0:		# Inlet blocked.
		return False
	while y < 999:
		if grid[y + 1, x] == 0:
			y += 1
		elif grid[y + 1, x - 1] == 0:
			x += -1
			y += 1
		elif grid[y + 1, x + 1] == 0:
			x += 1
			y += 1
		else:
			grid[y, x] = 1
			return True
	# Get to here and the sand falls off into the void.
	return False

def limit_bounds(a):
	xs,ys,zs = np.nonzero(a)
	return (min(xs), min(ys), min(zs), max(xs), max(ys), max(zs))

def settle_bricks():
	(lx, ly, lz, ux, uy, uz) = limit_bounds(grid)


def print_grid(g, bounds = None):
	fy,fx,ty,tx = bounds if bounds else limit_bounds(g)
	print(f"From: ({fx}, {fy}), To: ({tx}, {ty})")
	for y in range(fy, ty + 1, 1):
		s = []
		for x in range(fx, tx + 1, 1):
			ch = ' ' if g[y, x] == 0 else '#' if g[y, x] == 2 else 'o'
			s.append(ch)
		print("".join(s))

def part1_for(file_name):
	global grid
	grid = np.zeros((num_x, num_y, num_z), dtype = np.int16)
	global next_id
	next_id = 0
	lines = gulp_input(file_name)
	count = draw_bricks(lines)
	print('Drew ', count, ' bricks')

def part2_for(file_name):
	global grid
	grid = np.zeros((num_rows, num_cols), dtype = np.int16)
	lines = gulp_input(file_name)
	count = draw_rocks(lines)
	fy,fx,ty,tx = limit_bounds(grid)
	ty += 2
	draw_rocks([f"0,{ty} -> {num_cols - 1},{ty}"])
	print('Drew ', count, ' rock cells')
	print_grid(grid, (fy,fx,ty,tx))

	grains = 0
	while drop_sand_grain():
		grains += 1

	print('Dropped ', grains, ' sand grains.')
	print_grid(grid, (fy,fx,ty,tx))


# Main processing.
print('Advent of Code 2023 - Day 22, Part 1.')
print('Running test...')
part1_for('test22.txt')

print('Running full set...')
part1_for('test22.txt')
