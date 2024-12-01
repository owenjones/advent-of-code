from collections import defaultdict

input = open("input.txt").read()
l = list(map(lambda x: tuple(map((lambda i: int(i)), x.split())), input.split("\n")))
x, y = list(map(lambda x: sorted(x), list(zip(*l))))

a = sum([abs(x[i] - y[i]) for i in range(len(x))])
print(f"Part 1: {a}")

c = defaultdict(lambda: 0)
for n in y:
    c[n] += 1

b = sum([i * c[i] for i in x])
print(f"Part 2: {b}")
