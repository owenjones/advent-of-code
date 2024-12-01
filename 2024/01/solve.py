from collections import Counter

input = open("input.txt").read()
l = list(map(lambda x: map((lambda i: int(i)), x.split()), input.split("\n")))
x, y = list(map(lambda x: sorted(x), list(zip(*l))))

a = sum([abs(i - j) for (i, j) in zip(x, y)])
print(f"Part 1: {a}")

c = Counter(y)
b = sum([i * c[i] for i in x])
print(f"Part 2: {b}")
