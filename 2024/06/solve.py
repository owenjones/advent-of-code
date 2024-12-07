class Lab:
    def __init__(self, input):
        self.grid = list(map(lambda x: list(x), input.split("\n")))
        self.xdim = len(self.grid[0])
        self.ydim = len(self.grid)
        self._locate_guard()
        self.start = self.guard
        self.visited = set()
        self.loop = set()

    def reset(self):
        self.guard = self.start
        self.loop.clear()

    def _locate_guard(self):
        for y in range(self.ydim):
            for x in range(self.xdim):
                if self.grid[y][x] == "^":
                    self.guard = (x, y, 0)
                    self.grid[y][x] = "."
                    return

    def _next(self):
        c = [(0, -1), (1, 0), (0, 1), (-1, 0)][self.guard[2]]
        x = self.guard[0] + c[0]
        y = self.guard[1] + c[1]

        if not ((0 <= x < self.xdim) and 0 <= y < self.ydim):
            n = None
        else:
            n = self.grid[y][x]

        return (x, y, n)

    def initial_walk(self):
        self.visited.add((self.guard[0], self.guard[1]))
        (x, y, n) = self._next()

        while n != None:
            if n == ".":
                self.visited.add((x, y))
                self.guard = (x, y, self.guard[2])

            elif n == "#":
                self.guard = (self.guard[0], self.guard[1], (self.guard[2] + 1) % 4)

            (x, y, n) = self._next()

    def walk(self):
        self.loop.add(self.guard)
        (x, y, n) = self._next()

        while n != None:
            if n == ".":
                self.guard = (x, y, self.guard[2])

            elif n == "#":
                self.guard = (self.guard[0], self.guard[1], (self.guard[2] + 1) % 4)

                if self.guard in self.loop:
                    return 1

                self.loop.add(self.guard)

            (x, y, n) = self._next()

        return 0


input = open("input.txt").read()

lab = Lab(input)
lab.initial_walk()
total = len(lab.visited)
print(f"Part 1: {total}")

count = 0
locations = lab.visited
for l in locations:
    lab.reset()
    lab.grid[l[1]][l[0]] = "#"
    count += lab.walk()
    lab.grid[l[1]][l[0]] = "."

print(f"Part 2: {count}")
