import re

input = open("input.txt").read().split("\n")
robots = list(
    map(
        lambda x: tuple(
            map(
                int,
                re.fullmatch(r"^p=([-\d]+),([-\d]+) v=([-\d]+),([-\d]+)$", x).groups(),
            )
        ),
        input,
    )
)
xdim = max([i[0] for i in robots]) + 1
ydim = max([i[1] for i in robots]) + 1

shift = 100
q = [0, 0, 0, 0]
for x, y, vx, vy in robots:
    x = (x + (shift * vx)) % xdim
    y = (y + (shift * vy)) % ydim

    if x < xdim // 2 and y < ydim // 2:
        q[0] += 1

    elif x > xdim // 2 and y < ydim // 2:
        q[1] += 1

    elif x < xdim // 2 and y > ydim // 2:
        q[2] += 1

    elif x > xdim // 2 and y > ydim // 2:
        q[3] += 1

factor = q[0] * q[1] * q[2] * q[3]
print(f"Part 1: {factor}")

shift = 0
nr = len(robots)
while True:
    ps = set()

    for x, y, vx, vy in robots:
        x = (x + (shift * vx)) % xdim
        y = (y + (shift * vy)) % ydim

        if (x, y) in ps:
            break

        ps.add((x, y))

    if len(ps) == nr:
        # g = [[" " for _ in range(xdim)] for _ in range(ydim)]
        # for x, y in ps:
        #     g[y][x] = "*"

        # for r in g:
        #     print("".join(r))

        break

    shift += 1

print(f"Part 2: {shift}")
