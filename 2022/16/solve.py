from pathlib import Path
import re
import networkx as nx


def extract_valves(input: str) -> dict[tuple[int, list[str]]]:
    template = (
        "Valve (\w{2}) has flow rate=(\d+); tunnel([s]?) lead([s]?) to valve([s]?) (.*)"
    )
    pattern = re.compile(template)

    valves = {}
    with Path(input).open() as file:
        for v in file.readlines():
            search = pattern.search(v)
            valves[search[1]] = (
                int(search[2]),
                [x.strip() for x in search[6].split(", ")],
            )

    # An undirected graph containing all valves with unit distance between them
    G = nx.Graph()
    G.add_nodes_from(valves)

    for (source, (flow, children)) in valves.items():
        for child in children:
            G.add_edge(source, child)

    # Find shortest path between each pair of nodes - this is equal to time
    # needed to travel between the two nodes
    shortest_paths = nx.floyd_warshall(G)

    # Remove all valves not contributing flow
    targets = dict(filter(lambda x: x[1][0] > 0, valves.items()))
    targets = list(targets.keys())

    return (valves, targets, shortest_paths)


def valve_search(
    valves: dict[tuple[int, list[str]]],
    root: str,
    path: tuple,
    targets: list[str],
    cache: dict[tuple],
    shortest_paths: dict[dict],
    max_time: int,
) -> None:
    path = path + (root,) if path else (root,)
    current = cache.get(hash(path), None)
    if current:
        (current_time, current_flow, _) = current

        if current_time >= max_time:
            return
    else:
        current_time = 0
        current_flow = 0

    nodes = targets.copy()
    if root != "AA":
        nodes.remove(root)

    for node in nodes:
        if node in path:
            continue

        dt = int(shortest_paths[root][node] + 1)
        time = current_time + dt

        if time >= max_time:
            continue

        df = (max_time - time) * valves[node][0]
        flow = current_flow + df

        new_path = path + (node,)
        save = list(new_path)
        save.remove("AA")
        cache[hash(new_path)] = (time, flow, save)

        valve_search(valves, node, path, targets, cache, shortest_paths, max_time)


def part_a(
    valves: dict[tuple[int, list[str]]], targets: list[str], shortest_paths: dict[dict]
) -> int:
    cache = dict()
    valve_search(valves, "AA", None, targets, cache, shortest_paths, 30)
    cache = sorted(cache.items(), key=lambda x: x[1][1], reverse=True)
    return cache[0][1][1]


def part_b(
    valves: dict[tuple[int, list[str]]],
    targets: list[str],
    shortest_paths: dict[dict],
    part_a: int,
) -> int:
    cache = dict()
    valve_search(valves, "AA", None, targets, cache, shortest_paths, 26)
    cache = dict(sorted(cache.items(), key=lambda x: x[1][2], reverse=True))

    max_flow = 0

    for outer in cache.items():
        for inner in cache.items():
            if outer == inner:
                continue

            flow = outer[1][1] + inner[1][1]
            if flow <= part_a or flow <= max_flow:
                # going to assume result in part 2 is better than in part 1...
                continue

            # check valves don't appear in both sides (can't open a valve twice)
            if not any(x in outer[1][2] for x in inner[1][2]):
                max_flow = flow

    return max_flow


if __name__ == "__main__":
    input = "input.txt"
    (valves, targets, shortest_paths) = extract_valves(input)

    # Part 1 - 2253
    a = part_a(valves, targets, shortest_paths)
    print(f"Part 1: Most pressure that can be released = { a }")

    # Part 2 - 2838
    b = part_b(valves, targets, shortest_paths, a)
    print(f"Part 2: Most pressure that can be released = { b }")
