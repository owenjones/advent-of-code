from collections import deque


def part1(input):
    files = deque(enumerate(map(int, input[::2])))
    spaces = deque(map(int, input[1::2]))

    allocated = []
    while files:
        l = files.popleft()
        s = spaces.popleft()

        for _ in range(l[1]):
            allocated.append(l[0])

        if len(files) <= 0:
            break

        (i, v) = files.pop()
        while s > 0:
            if v > 0:
                allocated.append(i)
                v -= 1

            if v == 0:
                if len(files) <= 0:
                    break

                (i, v) = files.pop()

            s -= 1

        if v > 0:
            files.append((i, v))

    return sum(map(lambda x: x[0] * x[1], enumerate(allocated)))


def part2(input):
    files = []
    spaces = []
    disk = []

    for i, v in enumerate(input):
        if i % 2:
            spaces.append((int(v), len(disk)))
            disk += [-1 for _ in range(int(v))]
        else:
            files.append((int(i // 2), int(v), len(disk)))
            disk += [int(i // 2) for _ in range(int(v))]

    while files:
        f = files.pop()

        for i, s in enumerate(spaces):
            if f[2] < s[1]:
                break

            if s[0] >= f[1]:
                disk[f[2] : f[2] + f[1]] = [-1] * f[1]
                disk[s[1] : s[1] + f[1]] = [f[0]] * f[1]

                if s[0] > f[1]:
                    spaces[i] = (s[0] - f[1], s[1] + f[1])
                else:
                    spaces.pop(i)
                break

    return sum(disk[i] * i for i in range(len(disk)) if disk[i] > 0)


input = open("input.txt").read()
print(f"Part 1: {part1(input)}")
print(f"Part 2: {part2(input)}")
