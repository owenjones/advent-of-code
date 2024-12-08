from collections import defaultdict
from itertools import combinations

grid = open("input.txt").read().split("\n")
ydim = len(grid)
xdim = len(grid[0])

sets = defaultdict(set)
for y in range(ydim):
    for x in range(xdim):
        if grid[y][x] != ".":
            sets[grid[y][x]].add((x, y))

antinodes = set()
for antennas in sets.values():
    pairs = list(combinations(antennas, 2))
    for pair in pairs:
        d = (pair[1][0] - pair[0][0], pair[1][1] - pair[0][1])

        a = (pair[0][0] - d[0], pair[0][1] - d[1])
        b = (pair[1][0] + d[0], pair[1][1] + d[1])

        if 0 <= a[0] < xdim and 0 <= a[1] < ydim:
            antinodes.add(a)

        if 0 <= b[0] < xdim and 0 <= b[1] < ydim:
            antinodes.add(b)

total = len(antinodes)
print(f"Part 1: {total}")

antinodes = set()
for antennas in sets.values():
    pairs = list(combinations(antennas, 2))
    for pair in pairs:
        antinodes.add(pair[0])
        antinodes.add(pair[1])

        d = (pair[1][0] - pair[0][0], pair[1][1] - pair[0][1])

        (x, y) = (pair[0][0] - d[0], pair[0][1] - d[1])
        while 0 <= x < xdim and 0 <= y < ydim:
            antinodes.add((x, y))
            (x, y) = (x - d[0], y - d[1])

        (x, y) = (pair[1][0] + d[0], pair[1][1] + d[1])
        while 0 <= x < xdim and 0 <= y < ydim:
            antinodes.add((x, y))
            (x, y) = (x + d[0], y + d[1])

total = len(antinodes)
print(f"Part 2: {total}")
