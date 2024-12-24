from collections import deque
import re


def forwards(initial, gates):
    available = set()
    value = {}
    for i in initial.split("\n"):
        k, v = i.split(": ")
        value[k] = int(v)
        available.add(k)

    process = deque(gates)
    output = {}
    while process:
        x, op, y, z = process.popleft()

        if x in available and y in available:
            match op:
                case "AND":
                    value[z] = value[x] & value[y]
                case "OR":
                    value[z] = value[x] | value[y]
                case "XOR":
                    value[z] = value[x] ^ value[y]

            if z.startswith("z"):
                output[int(z[1:])] = value[z]
            available.add(z)

        else:
            process.append((x, op, y, z))

    return int(
        "".join(
            list(
                map(
                    lambda x: str(x[1]),
                    sorted(output.items(), key=lambda x: x[0], reverse=True),
                )
            )
        ),
        2,
    )


def backwards(gates):
    highest = 0
    for _, _, _, z in gates:
        if z.startswith("z"):
            n = int(z[1:])
            highest = max(n, highest)

    highest = f"z{highest}"

    wrong = set()
    for x, op, y, z in gates:
        if z.startswith("z") and op != "XOR" and z != highest:
            wrong.add(z)

        if (
            op == "XOR"
            and not x[0].startswith(("x", "y", "z"))
            and not y[0].startswith(("x", "y", "z"))
            and not z[0].startswith(("x", "y", "z"))
        ):
            wrong.add(z)

        elif op == "AND" and "x00" not in [x, y]:
            for x, op, y, _ in gates:
                if (z == x or z == y) and op != "OR":
                    wrong.add(z)

        elif op == "XOR":
            for x, op, y, _ in gates:
                if (z == x or z == y) and op == "OR":
                    wrong.add(z)

    return ",".join(sorted(wrong))


initial, gates = open("input.txt").read().split("\n\n")
gates = list(
    map(lambda g: re.findall(r"(.*) (.*) (.*) -> (.*)", g)[0], gates.split("\n"))
)
print(f"Part 1: {forwards(initial, gates)}")
print(f"Part 2: {backwards(gates)}")
