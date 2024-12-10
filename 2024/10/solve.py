from collections import deque

input = open("input.txt").read().split("\n")
grid = list(map(lambda x: [int(y) for y in x], map(list, input)))
ydim = len(grid)
xdim = len(grid[0])

starts = set()
for y in range(ydim):
    for x in range(xdim):
        if grid[y][x] == 0:
            starts.add((x, y))

scores = []
ratings = []
neighbours = [(1, 0), (0, 1), (-1, 0), (0, -1)]
for start in starts:
    rating = 0
    searches = deque([(start, 0)])
    end = set()
    while searches:
        (position, height) = searches.popleft()
        for next in neighbours:
            (x, y) = (position[0] + next[0], position[1] + next[1])
            if not (0 <= x < xdim and 0 <= y < ydim):
                continue

            next = grid[y][x]
            if (height + 1) != next:
                continue

            if next == 9:
                rating += 1
                end.add((x, y))
                continue

            searches.append(((x, y), next))

    scores.append(len(end))
    ratings.append(rating)

total = sum(scores)
print(f"Part 1: {total}")

total = sum(ratings)
print(f"Part 2: {total}")
