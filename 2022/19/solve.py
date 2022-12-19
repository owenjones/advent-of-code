from pathlib import Path
import re
from pulp import LpProblem, LpVariable, LpMaximize, LpInteger, lpSum, PULP_CBC_CMD

# Based on Two Stage Production Planning Problem:
# https://coin-or.github.io/pulp/CaseStudies/a_two_stage_production_planning_problem.html


def simulate(blueprint: str, minutes: int) -> int:
    # extract details from blueprint
    # (assume each robot is built from same type of ingredients, just different numbers?)
    template = "Blueprint ([-+]?\d+): Each ore robot costs ([-+]?\d+) ore. Each clay robot costs ([-+]?\d+) ore. Each obsidian robot costs ([-+]?\d+) ore and ([-+]?\d+) clay. Each geode robot costs ([-+]?\d+) ore and ([-+]?\d+) obsidian"

    pattern = re.compile(template)
    (
        id,
        cost_ore_ore,
        cost_clay_ore,
        cost_obsidian_ore,
        cost_obsidian_clay,
        cost_geode_ore,
        cost_geode_obsidian,
    ) = [int(x) for x in pattern.search(blueprint).groups()]

    # setup the problem
    times = list(range(minutes + 1))

    model = LpProblem("GeodeMining", LpMaximize)

    ore = LpVariable.dicts("ore", lowBound=0, cat=LpInteger, indices=times)
    clay = LpVariable.dicts("clay", lowBound=0, cat=LpInteger, indices=times)
    obsidian = LpVariable.dicts("obsidian", lowBound=0, cat=LpInteger, indices=times)
    geode = LpVariable.dicts("geode", lowBound=0, cat=LpInteger, indices=times)

    ore_r = LpVariable.dicts("ore_r", lowBound=0, cat=LpInteger, indices=times)
    clay_r = LpVariable.dicts("clay_r", lowBound=0, cat=LpInteger, indices=times)
    obsidian_r = LpVariable.dicts(
        "obsidian_r", lowBound=0, cat=LpInteger, indices=times
    )
    geode_r = LpVariable.dicts("geode_r", lowBound=0, cat=LpInteger, indices=times)

    #  set initial state
    ore[0] = 0
    clay[0] = 0
    obsidian[0] = 0
    geode[0] = 0

    ore_r[0] = 1
    clay_r[0] = 0
    obsidian_r[0] = 0
    geode_r[0] = 0

    # simulate each minute of the problem
    for x in range(minutes):
        # amount of each ore = previous amount + any mined by that resources
        # mining robots - the amount used to make any new robots
        model += (
            ore[x + 1]
            == ore[x]
            + ore_r[x]
            - (ore_r[x + 1] - ore_r[x]) * cost_ore_ore
            - (clay_r[x + 1] - clay_r[x]) * cost_clay_ore
            - (obsidian_r[x + 1] - obsidian_r[x]) * cost_obsidian_ore
            - (geode_r[x + 1] - geode_r[x]) * cost_geode_ore
        )

        model += (
            clay[x + 1]
            == clay[x]
            + clay_r[x]
            - (obsidian_r[x + 1] - obsidian_r[x]) * cost_obsidian_clay
        )

        model += (
            obsidian[x + 1]
            == obsidian[x]
            + obsidian_r[x]
            - (geode_r[x + 1] - geode_r[x]) * cost_geode_obsidian
        )

        model += geode[x + 1] == geode[x] + geode_r[x]

        # add constraints so we can't use more resources than we have building robots
        model += (
            ore[x]
            >= (ore_r[x + 1] - ore_r[x]) * cost_ore_ore
            + (clay_r[x + 1] - clay_r[x]) * cost_clay_ore
            + (obsidian_r[x + 1] - obsidian_r[x]) * cost_obsidian_ore
            + (geode_r[x + 1] - geode_r[x]) * cost_geode_ore
        )

        model += clay[x] >= (obsidian_r[x + 1] - obsidian_r[x]) * cost_obsidian_clay

        model += obsidian[x] >= (geode_r[x + 1] - geode_r[x]) * cost_geode_obsidian

        # add constraints so number of robots only increase or stay the same
        model += ore_r[x + 1] >= ore_r[x]
        model += clay_r[x + 1] >= clay_r[x]
        model += obsidian_r[x + 1] >= obsidian_r[x]
        model += geode_r[x + 1] >= geode_r[x]

        # final constraint - we can only make one robot at a time
        model += (
            ore_r[x + 1] + clay_r[x + 1] + obsidian_r[x + 1] + geode_r[x + 1]
        ) <= (ore_r[x] + clay_r[x] + obsidian_r[x] + geode_r[x] + 1)

    model += lpSum(geode)
    model.solve(PULP_CBC_CMD(msg=0))

    return (geode[minutes].value(), id)


def part_a(blueprints: list[str]) -> None:
    simulations = [simulate(blueprint, 24) for blueprint in blueprints]
    qualities = [(int(geodes) * id) for (geodes, id) in simulations]
    total = sum(qualities)
    print(f"Part 1: Sum of qualities of all blueprints = { total }")


def part_b(blueprints: list[str]) -> None:
    simulations = [simulate(blueprint, 32) for blueprint in blueprints[0:3]]
    geodes = [int(geodes) for (geodes, id) in simulations]
    total = geodes[0] * geodes[1] * geodes[2]
    print(f"Part 2: Product of first three blueprints = { total }")


if __name__ == "__main__":
    input = "input.txt"

    with Path(input).open() as file:
        blueprints = [blueprint for blueprint in file.readlines()]

    part_a(blueprints)  # 1404
    part_b(blueprints)  # 5880
