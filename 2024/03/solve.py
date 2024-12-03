import re

input = open("input.txt").read().replace("\n", "")


def multiplications(input):
    instructions = re.findall(r"mul\(([\d]+),([\d]+)\)", input)
    return sum(map(lambda x: (int(x[0]) * int(x[1])), instructions))


total = multiplications(input)
print(f"Part 1: {total}")

input = "do()" + input + "don't()"
correct = re.findall(r"do\(\)(.+?(?!=don't\(\)))don't\(\)", input)
total = multiplications("".join(correct))
print(f"Part 2: {total}")
