def process_input(file):
    input = open(file).read().split("\n\n")
    locks = []
    keys = []

    for block in input:
        block = block.split("\n")
        lines = [list(x) for x in zip(*block)]
        counts = tuple([line.count("#") - 1 for line in lines])

        if block[0] == "#####":
            locks.append(counts)
        else:
            keys.append(counts)

    return locks, keys


def compatable(lock, key):
    return not any(lock[i] + key[i] > 5 for i in range(len(lock)))


locks, keys = process_input("input.txt")
fits = 0
for lock in locks:
    for key in keys:
        if compatable(lock, key):
            fits += 1

print(f"Part 1: {fits}")
