#
# Day 18 - Lavaduct lagoon
#

import re

def read_input(file_name):
    with open(file_name, 'r') as infile:
        return list(infile)

def process_line(line):
    m = re.match(r"(?P<heading>[RLUD]) (?P<distance>\d+) \(#(?P<hexcode>[0-9a-f]+)\)", line)
    return (m['heading'], int(m['distance']), m['hexcode']) if m else None

def neighbours(p):
    x,y = p
    nl = [(x + dx, y + dy) for dx,dy in [(-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0),(1,1)] ]
    return nl

def part1(file_name):

    # Read file and trace out instructions populating a set of locations visited.
    # (value 1 indicates on the border of the lagoon)
    lines = read_input(file_name)
    p = (0,0)
    visits = {p : 1}
    instructions = [process_line(line) for line in lines]
    for (hdg, dist, _) in instructions:
        for i in range(dist):
            if hdg == 'R':
                p = (p[0] + 1, p[1])
            elif hdg == 'L':
                p = (p[0] - 1, p[1])
            elif hdg == 'U':
                p = (p[0], p[1] - 1)
            elif hdg == 'D':
                p = (p[0], p[1] + 1)
            else:
                print("Error in instruction - unrecognized heading", hdg)
                return
            visits[p] = 1
    
    # Find bounding box for locations visited.
    min_x = max_x = min_y = max_y = 0
    for x,y in visits:
        if x < min_x:
            min_x = x
        elif x > max_x:
            max_x = x
        if y < min_y:
            min_y = y
        elif y > max_y:
            max_y = y

    # Find the edges (outside the lagoon).
    edges = []
    for x in range(min_x, max_x + 1):
        if (x,min_y) not in visits:
            edges.append((x,min_y))
        if (x,max_y) not in visits:
            edges.append((x,max_y))
    for y in range(min_y + 1, max_y):
        if (min_x,y) not in visits:
            edges.append((min_x,y))
        if (max_x,y) not in visits:
            edges.append((max_x,y))

    # Spread outwards from the edges to neighbouring squares.
    while len(edges) > 0:
        e = edges.pop()
        visits[e] = 2       # 2 = outside
        nlist = [(x,y) for x,y in neighbours(e) if x >= min_x and x <= max_x and y >= min_y and y <= max_y]
        for n in nlist:
            x,y = n
            if n not in visits and n not in edges:
                edges.append(n)
    
    # Scan the rest of the grid and mark up as insiders.
    label = 4
    for x in range(min_x, max_x + 1):
        for y in range(min_y, max_y + 1):
            s = x,y
            if s not in visits:
                starts = [s]
                while len(starts) > 0:
                    t = starts.pop()
                    visits[t] = label
                    nlist = neighbours(t)
                    for n in nlist:
                        x,y = n
                        if n not in visits and n not in starts:
                            starts.append(n)
                label += 1

    # Counts for all the regions:
    counts = { c+1:0 for c in range(label) }
    for x in range(min_x, max_x + 1):
        for y in range(min_y, max_y + 1):
            counts[visits[(x,y)]] += 1

    print("Resulting counts:\n", counts)
    return 0


def part2(file_name):
    pass

# Main.
print('Advent of Code 2023 - Day 18, Part 1.')
print('Running test...')
answer = part1('test18.txt')
print(f"Answer is: {answer}")

print('Running full set...')
part1('input18.txt')
