input = map(lambda x: x.split("\n"), open("input.txt").read().split("\n\n"))
locks = []
keys = []

for block in input:
    lines = [list(x) for x in zip(*block)]
    counts = tuple([line.count("#") - 1 for line in lines])
    locks.append(counts) if block[0] == "#####" else keys.append(counts)

fits = 0
for lock in locks:
    for key in keys:
        if all(lock[i] + key[i] <= 5 for i in range(len(lock))):
            fits += 1

print(f"Part 1: {fits}")
