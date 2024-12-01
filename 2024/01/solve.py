from collections import defaultdict

input = open("input.txt").read()
l = list(map(lambda x: tuple(map((lambda i: int(i)), x.split())), input.split("\n")))
m = list(map(lambda x: sorted(x), list(zip(*l))))

a = sum([abs(m[0][i] - m[1][i]) for i in range(len(m[0]))])
print(f"Part 1: {a}")

c = defaultdict(lambda: 0)
for n in m[1]:
    c[n] += 1

b = sum([x * c[x] for x in m[0]])
print(f"Part 2: {b}")
