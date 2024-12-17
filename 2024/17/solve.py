import re

def run(program, a, b, c):
    i = 0
    out = []
    while i < len(program) - 1:
        (o, l) = (program[i], program[i+1])

        match(l):
            case 0: v = 0
            case 1: v = 1
            case 2: v = 2
            case 3: v = 3
            case 4: v = a
            case 5: v = b
            case 6: v = c
            case 7: exit("Invalid program")

        match(o):
            case 0: a = a // 2**v
            case 1: b = b ^ l
            case 2: b = v % 8
            case 3: i = i if a == 0 else l
            case 4: b = b ^ c
            case 5: out.append(v % 8)
            case 6: b = a // 2**v
            case 7: c = a // 2**v
        
        if o != 3 or (o == 3 and a == 0):
            i += 2

    return ",".join(map(str, out))

input = re.fullmatch(r"^Register A: (.*)\nRegister B: (.*)\nRegister C: (.*)\n\nProgram: (.*)$", open("input.txt").read()).groups()
a, b, c = map(int, input[0:3])
program = list(map(int, input[3].split(",")))

out = run(program, a, b, c)
print(f"Part 1: {out}")
