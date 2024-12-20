from heapq import heappop, heappush
from collections import defaultdict
from math import inf

grid = open("input.txt").read().split("\n")
ydim = len(grid)
xdim = len(grid[0])
start = [(x, y) for x in range(xdim) for y in range(ydim) if grid[y][x] == "S"][0]
end = [(x, y) for x in range(xdim) for y in range(ydim) if grid[y][x] == "E"][0]

def search():
    costs = defaultdict(lambda: inf)
    costs[start] = 0
    best = 0
    route = []

    heap = [(0, start, [start])]
    while heap:
        (cost, (x, y), path) = heappop(heap)

        if (x, y) == end:
            route = path
            break

        if best == 0 or cost < best:
            for dx, dy in ((1, 0), (0, 1), (-1, 0), (0, -1)):
                nx, ny = x + dx, y + dy

                if grid[ny][nx] == "#":
                    continue
                
                ncost = cost + 1

                if costs[(nx, ny)] >= ncost:
                    costs[(nx, ny)] = ncost
                    heappush(heap, (ncost, (nx, ny), path + [(nx, ny)]))

    return route, costs

def cheatlist():
    n = [(2, 0), (0, 2), (-2, 0), (0, -2),(1, 0), (0, 1), (-1, 0), (0, -1)]
    for x in range(-20, 21):
        for y in range(-20, 21):
            m = abs(x) + abs(y)
            if 3 <= m <= 20:
                n.append((x, y))

    return n

route, costs = search()
cheats = cheatlist()

path = set(route)
smallcheats = 0
bigcheats = 0
for x, y in path:
    for dx, dy in cheats:
        nx, ny = x + dx, y + dy
        a = costs[(x, y)]

        if (nx, ny) in path:
            b = costs[(nx, ny)]
            c = abs(dx) + abs(dy)
            
            if b - a >= 100 + c:
                if c == 2:
                    smallcheats += 1

                bigcheats += 1
            
print(f"Part 1: {smallcheats}")
print(f"Part 2: {bigcheats}")