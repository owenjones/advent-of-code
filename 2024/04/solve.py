from collections import defaultdict

input = list(map(lambda x: x.strip(), open("input.txt").readlines()))
transpose = list(map(lambda x: "".join(x), list(zip(*input))))

positive = defaultdict(list)
negative = defaultdict(list)
for x in range(len(input)):
    for y in range(len(input)):
        positive[x + y].append(input[x][y])
        negative[x - y].append(input[x][y])

positive = list(map(lambda x: "".join(x), positive.values()))
negative = list(map(lambda x: "".join(x), negative.values()))

search = [*input, *transpose, *positive, *negative]
total = sum(map(lambda x: x.count("XMAS") + x.count("SAMX"), search))
print(f"Part 1: {total}")

total = 0
for x in range(1, len(input) - 1):
    for y in range(1, len(input) - 1):
        if (
            input[x][y] == "A"
            and sorted([input[x - 1][y - 1], input[x + 1][y + 1]]) == ["M", "S"]
            and sorted([input[x + 1][y - 1], input[x - 1][y + 1]]) == ["M", "S"]
        ):
            total += 1

print(f"Part 2: {total}")
