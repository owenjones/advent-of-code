from heapq import heappush, heappop

def simulate(steps, size):
    grid = [[ 0 for _ in range(size + 1) ] for _ in range(size + 1) ]
    for i in range(steps):
        (x, y) = bytes[i]
        grid[y][x] = 1

    return grid

def search(grid):
    size = len(grid) - 1
    end = (size, size)
    neighbours = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    heap = [(0, (0, 0, set([(0, 0)])))]

    while heap:
        (s, (x, y, path)) = heappop(heap)

        if (x, y) == end:
            return s

        for (dx, dy) in neighbours:
            nx, ny, ns = x + dx, y + dy, s + 1

            if (
                (nx, ny) in path or
                not (0 <= nx <= size and 0 <= ny <= size) or
                grid[ny][nx] == 1
            ):
                continue

            path.add((nx, ny))
            heappush(heap, (ns, (nx, ny, path)))

    return None
    

def predictFuture(steps, size):
    for i in reversed(range(steps, len(bytes))):
        grid = simulate(i, size)
        steps = search(grid)

        if steps:
            return ",".join(map(str, bytes[i]))


FILE = "input.txt"
SIZE = 70
SIM = 1024
bytes = list(map(lambda x: tuple(map(int, x.split(","))), open(FILE).read().split("\n")))

grid = simulate(SIM, SIZE)
steps = search(grid)
print(f"Part 1: {steps}")

blocker = predictFuture(SIM, SIZE)
print(f"Part 2: {blocker}")