from functools import cache

numeric = ("789", "456", "123", "#0A")
directional = ("#^A", "<v>")
keypads = {"numeric": numeric, "directional": directional}
positions = {
    "numeric": {
        numeric[y][x]: (y, x)
        for y in range(len(numeric))
        for x in range(len(numeric[0]))
    },
    "directional": {
        directional[y][x]: (y, x)
        for y in range(len(directional))
        for x in range(len(directional[0]))
    },
}


@cache
def shortest(a, b, k):
    k
    y1, x1 = positions[k][a]
    y2, x2 = positions[k][b]
    dy, dx = y2 - y1, x2 - x1

    y = "v" * dy if dy >= 0 else "^" * -dy
    x = ">" * dx if dx >= 0 else "<" * -dx

    if x == 0 and y == 0:
        return [""]

    elif x == 0:
        return [y]

    elif y == 0:
        return [x]

    elif keypads[k][y1][x2] == "#":
        return [y + x]

    elif keypads[k][y2][x1] == "#":
        return [x + y]

    else:
        return [x + y, y + x]


@cache
def presses(code, depth):
    if depth == 1:
        return len(code)

    keypad = "numeric" if code[0] in "0123456789" else "directional"
    count = 0
    for a, b in zip("A" + code, code):
        path = shortest(a, b, keypad)
        count += min(presses(p + "A", depth - 1) for p in path)

    return count


def complexity(code, robots):
    return presses(code, robots) * int(code[:-1])


codes = open("input.txt").read().split("\n")

total = sum(complexity(code, 4) for code in codes)
print(f"Part 1: {total}")

total = sum(complexity(code, 27) for code in codes)
print(f"Part 2: {total}")
