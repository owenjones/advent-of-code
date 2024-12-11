from collections import defaultdict

stones = list(map(int, open("input.txt").read().split()))
counts = defaultdict(lambda: 0)
for s in stones:
    counts[s] += 1

part1 = 0

for i in range(75):
    out = defaultdict(lambda: 0)

    for k, v in counts.items():
        if k == 0:
            out[1] += v

        elif len(str(k)) % 2 == 0:
            s = str(k)
            n = int(len(s) / 2)
            out[int(s[:n])] += v
            out[int(s[n:])] += v

        else:
            out[(k * 2024)] += v

    counts = out

    if i == 24:
        part1 = sum(counts.values())


total = sum(counts.values())
print(f"Part 1: {part1}")
print(f"Part 2: {total}")
