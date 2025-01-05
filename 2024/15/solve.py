changes = {">": (1, 0), "v": (0, 1), "<": (-1, 0), "^": (0, -1)}


def biggify(grid):
    big = []
    for row in grid:
        line = ""
        for c in row:
            match c:
                case "#":
                    line += "##"
                case "O":
                    line += "[]"
                case ".":
                    line += ".."
                case "@":
                    line += "@."
        big.append(list(line))

    return big


def coordinates(grid):
    count = 0
    for y, row in enumerate(grid):
        for x, c in enumerate(row):
            if c in ("O", "["):
                count += x + 100 * y

    return count


def simulate(grid, moves):
    x, y = [
        (x, y)
        for y in range(len(grid))
        for x in range(len(grid[0]))
        if grid[y][x] == "@"
    ][0]
    grid[y][x] = "."

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
    x, y = [
        (x, y)
        for y in range(len(grid))
        for x in range(len(grid[0]))
        if grid[y][x] == "@"
    ][0]
    grid[y][x] = "."

    for move in moves:
        dx, dy = changes[move]
        nx, ny = x + dx, y + dy

        if grid[ny][nx] == "#":
            continue

        elif grid[ny][nx] == ".":
            x, y = nx, ny
            continue

        elif move in "><":
            ox, oy = nx, ny

            while grid[ny][nx] in "[]":
                nx += dx

            if grid[ny][nx] == ".":
                while nx != ox:
                    grid[ny][nx] = grid[ny][nx - dx]
                    nx -= dx

                grid[oy][ox] = "."
                x = ox

        elif move in "^v":
            ox, oy = nx, ny
            ov = (nx, nx + 1) if grid[ny][nx] == "[" else (nx - 1, nx)
            all = set()
            xs = set(ov)
            stack = []

            while xs:
                for n in xs:
                    all.add((n, ny + dy))

                chunk = []
                nxs = set()
                line = [grid[ny + dy][n] for n in xs]

                if "#" in line:
                    stack = None
                    break

                elif line == ["."] * len(xs):
                    for n in xs:
                        all.add((n, ny + dy))
                        chunk.append(((n, ny), (n, ny + dy)))

                    stack.append(chunk)
                    break

                else:
                    for n in xs:
                        chunk.append(((n, ny), (n, ny + dy)))

                        match (grid[ny][n], grid[ny + dy][n]):
                            case ("[", "["):
                                nxs.add(n)
                                nxs.add(n + 1)

                            case ("]", "]"):
                                nxs.add(n - 1)
                                nxs.add(n)

                            case ("]", "["):
                                nxs.add(n)
                                nxs.add(n + 1)

                            case ("[", "]"):
                                nxs.add(n - 1)
                                nxs.add(n)

                stack.append(chunk)
                ny += dy
                xs = nxs

            if stack:
                for chunk in reversed(stack):
                    for (ax, ay), (bx, by) in chunk:
                        grid[by][bx] = grid[ay][ax]

                        if (ax, ay) not in all:
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
