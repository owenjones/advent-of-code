from pathlib import Path
import re
import networkx as nx
import itertools


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
    pressures: list[int],
    shortest_paths: dict[dict],
    max_time: int,
) -> None:
    path = path + (root,) if path else (root,)
    current = cache.get(hash(path), None)
    if current:
        (current_time, current_flow) = current

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

        if time > max_time:
            continue

        df = (max_time - time) * valves[node][0]
        flow = current_flow + df

        new_path = path + (node,)
        cache[hash(new_path)] = (time, flow)

        pressures.append(flow)
        valve_search(
            valves, node, path, targets, cache, pressures, shortest_paths, max_time
        )


def part_a(valves, targets, shortest_paths) -> int:
    # Work through nodes - cache time & max flow for each leg to speed
    # up calculating additional searches
    cache = dict()
    pressures = list()
    valve_search(valves, "AA", None, targets, cache, pressures, shortest_paths, 30)
    return max(pressures)


def part_b():
    # partition target node set, find max pressure for each half and sum
    # find max of summed pressures
    pass


if __name__ == "__main__":
    input = "input.txt"
    (valves, targets, shortest_paths) = extract_valves(input)

    # Part 1 - 2253
    pressure = part_a(valves, targets, shortest_paths)
    print(f"Part 1: Most pressure that can be released = { pressure }")

    # Part 2 -
    # pressure = part_b(valves)
