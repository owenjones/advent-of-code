input = open("input.txt").read()
reports = list(
    map(lambda x: list(map((lambda i: int(i)), x.split())), input.split("\n"))
)


def isSafe(report):
    l = list(map(lambda x: x[0] - x[1], zip(report, report[1:])))
    s = len(report) - 1
    p = len(list(filter(lambda x: x > 0, l))) == s
    n = len(list(filter(lambda x: x < 0, l))) == s
    v = max(map(abs, l)) <= 3
    return (p or n) and v


def dampened(report):
    s = isSafe(report)
    if s:
        return True
    else:
        for n in range(len(report)):
            s = isSafe(report[:n] + report[(n + 1) :])
            if s:
                return True
    return False


total = list(map(isSafe, reports)).count(True)
print(f"Part 1: {total}")

total = list(map(dampened, reports)).count(True)
print(f"Part 2: {total}")
