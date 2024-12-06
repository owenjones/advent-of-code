from collections import defaultdict
from functools import cmp_to_key

input = open("input.txt").read().split("\n\n")
pages = list(map(lambda x: list(map(int, x.split(","))), input[1].split("\n")))

rules = defaultdict(list)
for r in input[0].split("\n"):
    x = r.split("|")
    rules[int(x[0])].append(int(x[1]))

def isCorrect(p):
    l = []
    for n in p:
        if n in rules:
            for x in rules[n]:
                if x in l:
                    return False

        l.append(n)
    
    return True

correct = []
incorrect = []
for p in pages:
    c = isCorrect(p)
    if c:
        correct.append(p)
    else:
        incorrect.append(p)

total = sum(map(lambda x: x[int(len(x) / 2)], correct))
print(f"Part 1: {total}")

def rulesort(a, b):
    if a in rules:
        if b in rules[a]:
            return -1 
        return 1
    return 0

def makeCorrect(p):
    return sorted(p, key=cmp_to_key(rulesort))

total = sum(map(lambda x: x[int(len(x) / 2)], map(makeCorrect, incorrect)))
print(f"Part 2: {total}")