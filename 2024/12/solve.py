from collections import deque


def floodFill(x, y):
    region = grid[y][x]
    contents = set([(x, y)])

    search = deque([(x, y)])
    while search:
        s = search.popleft()
        for n in neighbours:
            x, y = s[0] + n[0], s[1] + n[1]

            if (x, y) in contents:
                continue

            if not ((0 <= x < xdim) and (0 <= y < ydim)):
                continue

            if grid[y][x] != region:
                continue

            contents.add((x, y))
            search.append((x, y))

    return contents


def findRegions():
    visited = set()
    regions = []

    for y in range(ydim):
        for x in range(xdim):
            if (x, y) not in visited:
                region = floodFill(x, y)
                visited.update(region)
                regions.append(region)

    return regions


grid = list(map(list, open("input.txt").read().split("\n")))
ydim = len(grid)
xdim = len(grid[0])
neighbours = [(1, 0), (0, 1), (-1, 0), (0, -1)]
regions = findRegions()

price = 0
for r in regions:
    perimeter = 0

    for p in r:
        for n in neighbours:
            if (p[0] + n[0], p[1] + n[1]) not in r:
                perimeter += 1

    price += perimeter * len(r)

print(f"Part 1: {price}")

price = 0
for r in regions:
    corners = 0
    visited = set()

    for p in r:
        for n in [(0.5, -0.5), (-0.5, 0.5), (0.5, 0.5), (-0.5, -0.5)]:
            x, y = p[0] + n[0], p[1] + n[1]

            if (x, y) in visited:
                continue

            visited.add((x, y))

            adjacent = sum(
                (x + dx, y + dy) in r
                for dx, dy in [(0.5, -0.5), (-0.5, 0.5), (0.5, 0.5), (-0.5, -0.5)]
            )

            if adjacent in [1, 3]:
                corners += 1

            elif adjacent == 2:
                pattern = [
                    (dx, dy) in r for dx, dy in [(x - 0.5, y - 0.5), (x + 0.5, y + 0.5)]
                ]

                if pattern == [True, True] or pattern == [False, False]:
                    corners += 2

    price += corners * len(r)

print(f"Part 2: {price}")
