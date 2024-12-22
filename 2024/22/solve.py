from collections import defaultdict


def generate(numbers):
    count = []
    total = defaultdict(lambda: 0)

    for n in numbers:
        d = n % 10
        magic = []
        for _ in range(2000):
            n = ((n * 64) ^ n) % 16777216
            n = ((n // 32) ^ n) % 16777216
            n = ((n * 2048) ^ n) % 16777216
            t = n % 10
            magic.append((t - d, t))
            d = t

        count.append(n)

        seen = set()
        for i in range(len(magic) - 4):
            sequence = tuple(m[0] for m in magic[i : i + 4])
            if sequence not in seen:
                seen.add(sequence)
                total[sequence] += magic[i + 3][1]

    return sum(count), max(total.values())


numbers = list(map(int, open("input.txt").read().split("\n")))
magic, bananas = generate(numbers)
print(f"Part 1: {magic}")
print(f"Part 2: {bananas}")
