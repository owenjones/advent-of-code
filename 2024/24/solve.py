from collections import deque
import re


def extractbits(d):
    return "".join(
        list(
            map(
                lambda x: str(x[1]),
                sorted(d.items(), key=lambda x: x[0], reverse=True),
            )
        )
    )


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

    return output


def backwards(gates):
    highest_z = "z45"
    wrong = set()
    for x, op, y, z in gates:
        if z.startswith("z") and op != "XOR" and z != highest_z:
            wrong.add(z)

        if (
            op == "XOR"
            and not x[0].startswith(("x", "y", "z"))
            and not y[0].startswith(("x", "y", "z"))
            and not z[0].startswith(("x", "y", "z"))
        ):
            wrong.add(z)

        if op == "AND" and "x00" not in [x, y]:
            for x2, op2, y2, _ in gates:
                if (z == x2 or z == y2) and op2 != "OR":
                    wrong.add(z)

        if op == "XOR":
            for x2, op2, y2, _ in gates:
                if (z == x2 or z == y2) and op2 == "OR":
                    wrong.add(z)

    return ",".join(sorted(wrong))


initial, gates = open("input.txt").read().split("\n\n")
gates = list(
    map(lambda g: re.findall(r"(.*) (.*) (.*) -> (.*)", g)[0], gates.split("\n"))
)

bits = forwards(initial, gates)
number = int(extractbits(bits), 2)
print(f"Part 1: {number}")

print(f"Part 2: {backwards(gates)}")
