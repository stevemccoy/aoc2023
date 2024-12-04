#
# Day 21 - Step Counter.
#

# Globals.
num_x = num_y = 0

# Read the contents of the given file.
def read_input(file_name):
	with open(file_name, 'r') as f:
		return [line.strip() for line in f if len(line) > 0]
	return False

def valid(x, y):
	return (x >= 0 and x < num_x and y >= 0 and y < num_y)

def get_finite(lines, x, y):
	if valid(x, y):
		return lines[y][x]
	return False

def find_start(lines):
	for y in range(num_y):
		if 'S' in lines[y]:
			x = lines[y].find('S')
			return (x, y)
	return False

def do_steps(lines, src, dest):
	(sx, sy) = src
	for (dx, dy) in [(-1,0), (1,0), (0,-1), (0,1)]:
		x = sx + dx
		y = sy + dy
		if get_finite(lines, x % num_x, y % num_y) in ['.','S']:
			dest[(x,y)] = 1

def do_steps_avoiding(lines, src, avoid_front, dest):
	(sx, sy) = src
	for (dx, dy) in [(-1,0), (1,0), (0,-1), (0,1)]:
		x = sx + dx
		y = sy + dy
		xr = x % num_x
		yr = y % num_y
		if get_finite(lines, xr, yr) in ['.','S']:
			if (x,y) not in avoid_front:
				dest[(x,y)] = 1

def print_grid(lines, hot_squares):
	for y in range(num_y):
		line = ['H' if (x,y) in hot_squares else get_finite(lines, x, y) for x in range(num_x)]
		print("".join(line))

def part1_for(file_name, num_steps):
	global lines, num_x, num_y
	lines = read_input(file_name)
	num_y = len(lines)
	num_x = len(lines[0])
	(x,y) = find_start(lines)
	frontier = {(x,y)}
	for s in range(num_steps):
		dest = {}		# Frontier after this step.
		for src in frontier:
			do_steps(lines, src, dest)
		frontier = dest
	# Done num_steps now - how many distinct destination points.
	count = len(frontier)
	print(f"The elf can reach a total of {count} plots in exactly {num_steps} steps.")
	return count

def extend_frontier(last_front, this_front):
	result = {}
	for src in this_front:
		do_steps_avoiding(lines, src, last_front, result)
	return result

def part2_for(file_name, stop_points):
	global lines, num_x, num_y
	lines = read_input(file_name)
	num_y = len(lines)
	num_x = len(lines[0])
	(x,y) = find_start(lines)
	num_steps = max(stop_points.keys())

	even_count = 0
	odd_count = 0
	last_front = {}
	this_front = {(x,y)}

	for s in range(num_steps):
		if s in stop_points:
			print_grid(lines, this_front)
			n = len(this_front) + (odd_count if s % 2 else even_count)
			print(f"Stop point {s} reached. Expected count = {stop_points[s]}. Actual count ={n}")

		next_front = extend_frontier(last_front, this_front)
		if s % 2:
			odd_count += len(this_front)
		else:
			even_count += len(this_front)
		last_front = this_front
		this_front = next_front

	# Done num_steps now - how many distinct destination points.
	count = len(this_front) + (odd_count if num_steps % 2 else even_count)
	print(f"The elf can reach a total of {count} plots in exactly {num_steps} steps.")
	return count


# Main processing.
print('Advent of Code 2023 - Day 21, Part 1.')
print('Running test...')
count = part1_for('test21.txt', 6)
print('Running full input...')
count = part1_for('input21.txt', 64)

print('Part 2.')
print('Running test...')
sp = {6:16, 10:50}
stop_points = {6:16, 10:50, 50:1594, 100:6536, 500:167004, 1000:668697, 5000:16733044}
count = part2_for('test21.txt', stop_points)

# # print('Running full input...')
# count = part2_for('input21.txt', 64)
