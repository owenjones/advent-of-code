import re

input = open("input.txt").read()


def pairs(input):
    instructions = re.findall(r"mul\(([\d]+),([\d]+)\)", input)
    return list(map(lambda x: (int(x[0]), int(x[1])), instructions))


total = sum(map(lambda x: x[0] * x[1], pairs(input)))
print(f"Part 1: {total}")

input = "do()" + input.replace("\n", "") + "don't()"
correct = re.findall(r"do\(\)(.+?(?!=don't\(\)))don't\(\)", input)
total = sum(map(lambda x: x[0] * x[1], pairs("".join(correct))))
print(f"Part 2: {total}")
