from collections import defaultdict

def parse(file):
    input = open(file).read().split("\n\n")
    towels = set(input[0].split(", "))
    designs = input[1].split("\n")
    maxt = max(map(len, towels))
    return towels, designs, maxt


def count(design):
    n = len(design)
    d = defaultdict(lambda: 0)
    d[0] = 1

    for i in range(1, n + 1):
        for j in range(1, maxt + 1):
            if i >= j and design[i - j:i] in towels:
                d[i] += d[i - j]

    return d[n]

towels, designs, maxt = parse("input.txt")
possible = list(map(count, designs))

numberPossible = sum(x > 0 for x in possible)
print(f"Part 1: {numberPossible}")

totalPossible = sum(possible)
print(f"Part 2: {totalPossible}")