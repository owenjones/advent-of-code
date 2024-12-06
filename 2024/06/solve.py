from collections import defaultdict

input = open("input.txt").read()

class Lab:
    def __init__(self, input):
        self.grid = list(map(lambda x: list(x), input.split("\n")))
        self.xdim = len(self.grid[0])
        self.ydim = len(self.grid)
        self._locate_guard()
        self.visited = defaultdict(lambda: False)
        self.loop = defaultdict(lambda: False)

    def _locate_guard(self):
        for y in range(self.ydim):
            for x in range(self.xdim):
                if self.grid[y][x] == "^":
                    self.guard = (x, y, 0)
                    self.grid[y][x] = "."
                    return

    def _get(self, x, y):
        if not ((0 <= x < self.xdim) and 0 <= y < self.ydim):
            return None
        
        return self.grid[y][x]

    def _next(self):
        match(self.guard[2]):
            case 0: c = (0, -1)
            case 1: c = (1, 0)
            case 2: c = (0, 1)
            case 3: c = (-1, 0)

        x = self.guard[0] + c[0]
        y = self.guard[1] + c[1]
        return (x, y)
    
    def walk(self):
        self.visited[(self.guard[0], self.guard[1])] = True
        self.loop[self.guard] = True
        (x, y) = self._next()
        n = self._get(x, y)

        while n != None:
            if n == ".":
                self.visited[(x, y)] = True
                self.guard = (x, y, self.guard[2])

            elif n == "#":
                r = (self.guard[2] + 1) % 4
                self.guard = (self.guard[0], self.guard[1], r)

            if self.loop[self.guard] == True:
                return 1

            self.loop[self.guard] = True
            (x, y) = self._next()
            n = self._get(x, y)

        return 0

    def count_visited(self):
        return len(self.visited.keys())

lab = Lab(input)
lab.walk()
total = lab.count_visited()
print(f"Part 1: {total}")

count = 0
locations = lab.visited.keys()
for l in locations:
    lab = Lab(input)
    lab.grid[l[1]][l[0]] = "#"
    count += lab.walk()

print(f"Part 2: {count}")