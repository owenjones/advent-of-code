from collections import deque

input = open("input.txt").read().split("\n")

equations = []
for l in input:
    x = l.split(": ")
    e = (int(x[0]), list(map(int, x[1].split())))
    equations.append(e)


def canBuild(equation, part2=False):
    target, numbers = equation
    numbers = list(reversed(numbers))
    queue = deque([(target, 0)])
    seen = set()
    while queue:
        l = queue.popleft()
        (t, d) = l

        if t == 0:
            return target

        elif l in seen or t < 0 or t > target or d >= len(numbers):
            continue

        seen.add(l)
        queue.append((t - numbers[d], d + 1))

        if t % numbers[d] == 0:
            queue.append((int(t / numbers[d]), d + 1))

        if part2:
            if str(t).endswith(str(numbers[d])):
                n = str(t)[: -len(str(numbers[d]))]
                if n:
                    queue.append((int(n), d + 1))

    return 0


def canBuild2(equation):
    return canBuild(equation, True)


total = sum(map(canBuild, equations))
print(f"Part 1: {total}")

total = sum(map(canBuild2, equations))
print(f"Part 2: {total}")
