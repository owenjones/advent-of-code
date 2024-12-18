from heapq import heappush, heappop

def search(bytes, steps, size):
    end = (size, size)
    neighbours = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    heap = [(0, (0, 0, set([(0, 0), *bytes[:steps]])))]

    while heap:
        (s, (x, y, path)) = heappop(heap)

        if (x, y) == end:
            return s

        for (dx, dy) in neighbours:
            nx, ny, ns = x + dx, y + dy, s + 1

            if (
                (nx, ny) in path or
                not (0 <= nx <= size and 0 <= ny <= size)
            ):
                continue

            path.add((nx, ny))
            heappush(heap, (ns, (nx, ny, path)))

    return None
    

def predictFuture(bytes, steps, size):
    for i in reversed(range(steps, len(bytes))):
        steps = search(bytes, i, size)

        if steps:
            return ",".join(map(str, bytes[i]))


FILE = "input.txt"
SIZE = 70
SIM = 1024
bytes = list(map(lambda x: tuple(map(int, x.split(","))), open(FILE).read().split("\n")))

steps = search(bytes, SIM, SIZE)
print(f"Part 1: {steps}")

blocker = predictFuture(bytes, SIM, SIZE)
print(f"Part 2: {blocker}")