changes = {">": (1, 0), "v": (0, 1), "<": (-1, 0), "^": (0, -1)}


def biggify(grid):
    extend = {"#": "##", "O": "[]", ".": "..", "@": "@."}
    return list(map(lambda row: list("".join([extend[c] for c in row])), grid))


def coordinates(grid):
    count = 0

    for y, row in enumerate(grid):
        for x, c in enumerate(row):
            if c in "O[":
                count += x + 100 * y

    return count


def start(grid):
    for y, row in enumerate(grid):
        for x, c in enumerate(row):
            if c == "@":
                grid[y][x] = "."
                return x, y


def simulate(grid, moves):
    x, y = start(grid)

    for move in moves:
        dx, dy = changes[move]
        nx, ny = x + dx, y + dy

        if grid[ny][nx] == ".":
            x, y = nx, ny
            continue

        elif grid[ny][nx] == "O":
            ox, oy = nx, ny

            while grid[ny][nx] == "O":
                nx += dx
                ny += dy

            if grid[ny][nx] == ".":
                grid[oy][ox] = "."
                grid[ny][nx] = "O"

                x, y = ox, oy

    return grid


def bigSimulate(grid, moves):
    x, y = start(grid)

    for move in moves:
        dx, dy = changes[move]
        nx, ny = x + dx, y + dy

        if grid[ny][nx] == "#":
            continue

        if grid[ny][nx] == ".":
            x, y = nx, ny
            continue

        ox, oy = nx, ny

        if move in "><":
            while grid[ny][nx] in "[]":
                nx += dx

            if grid[ny][nx] == ".":
                while nx != ox:
                    grid[ny][nx] = grid[ny][nx - dx]
                    nx -= dx

                grid[oy][ox] = "."
                x = ox

        elif move in "^v":
            boxes = set()
            stack = []
            frontier = set((nx, nx + 1) if grid[ny][nx] == "[" else (nx - 1, nx))

            while frontier:
                nfrontier = set()
                line = [grid[ny + dy][n] for n in frontier]

                if "#" in line:
                    stack = None
                    break

                boxes |= set((n, ny + dy) for n in frontier)

                if line == ["."] * len(frontier):
                    stack.append([((n, ny), (n, ny + dy)) for n in frontier])
                    break

                else:
                    chunk = []
                    for n in frontier:
                        chunk.append(((n, ny), (n, ny + dy)))

                        match (grid[ny][n], grid[ny + dy][n]):
                            case ("[", "["):
                                nfrontier.add(n)

                            case ("]", "]"):
                                nfrontier.add(n)

                            case ("]", "["):
                                nfrontier.add(n)
                                nfrontier.add(n + 1)

                            case ("[", "]"):
                                nfrontier.add(n - 1)
                                nfrontier.add(n)

                    stack.append(chunk)

                ny += dy
                frontier = nfrontier

            if stack:
                for chunk in reversed(stack):
                    for (ax, ay), (bx, by) in chunk:
                        grid[by][bx] = grid[ay][ax]

                        if (ax, ay) not in boxes:
                            grid[ay][ax] = "."

                y = oy

    return grid


grid, moves = open("input.txt").read().split("\n\n")
moves = "".join(moves.split("\n"))
small = list(map(list, grid.split("\n")))
big = biggify(small)

total = coordinates(simulate(small, moves))
print(f"Part 1: {total}")

total = coordinates(bigSimulate(big, moves))
print(f"Part 2: {total}")
