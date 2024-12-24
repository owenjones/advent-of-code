from collections import deque
import re

initial, gates = open("input.txt").read().split("\n\n")

available = set()
value = {}
for i in initial.split("\n"):
    k, v = i.split(": ")
    value[k] = int(v)
    available.add(k)

process = deque()
for g in gates.split("\n"):
    v = re.findall(r"(.*) (.*) (.*) -> (.*)", g)
    process.append(*v)

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

bits = "".join(
    list(
        map(
            lambda x: str(x[1]),
            sorted(output.items(), key=lambda x: x[0], reverse=True),
        )
    )
)
number = int(bits, 2)
print(f"Part 1: {number}")
