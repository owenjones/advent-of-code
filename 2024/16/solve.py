import heapq


def rotate(a, b):
    return {
        "E": {"N": 1, "E": 0, "S": 1, "W": 2},
        "S": {"N": 2, "E": 1, "S": 0, "W": 1},
        "W": {"N": 1, "E": 2, "S": 1, "W": 0},
        "N": {"N": 0, "E": 1, "S": 2, "W": 1},
    }[a][b]


grid = open("input.txt").read().split("\n")
ydim = len(grid)
xdim = len(grid[0])
start = [(x, y) for x in range(xdim) for y in range(ydim) if grid[y][x] == "S"][0]
end = [(x, y) for x in range(xdim) for y in range(ydim) if grid[y][x] == "E"][0]
neighbours = ((1, 0, "E"), (0, 1, "S"), (-1, 0, "W"), (0, -1, "N"))


def search():
    costs = {((start[0], start[1]), "E"): 0}
    best = 0
    seats = []
    heap = [(0, (start[0], start[1]), "E", [])]
    while heap:
        (cost, (x, y), d, path) = heapq.heappop(heap)

        if (x, y) == end:
            best = cost
            seats = seats + path

        if best == 0 or cost < best:
            for dx, dy, dd in neighbours:
                nx, ny = x + dx, y + dy

                if grid[ny][nx] == "#":
                    continue

                r = rotate(d, dd)
                ncost = cost + 1
                ncost += r * 1000

                if costs.get(((nx, ny), dd), ncost + 1) >= ncost:
                    costs[((nx, ny), dd)] = ncost
                    heapq.heappush(heap, (ncost, (nx, ny), dd, path + [(nx, ny)]))

    return (best, len(set(seats)) + 1)


best, seats = search()
print(f"Part 1: {best}")
print(f"Part 2: {seats}")
