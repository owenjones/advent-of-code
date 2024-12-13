import re

def play(game, part2 = False):
  (a, c, b, d, x, y) = game

  if part2:
    x += 10000000000000
    y += 10000000000000

  det = a * d - b * c
  buttonA = d * x - b * y
  buttonB = a * y - c * x

  if buttonA % det == 0 and buttonB % det == 0:
    return int((3 * buttonA + buttonB) / det)
  
  return 0

input = open("input.txt").read().split("\n\n")
r = r"^Button A: X\+([\d]+), Y\+([\d]+)\nButton B: X\+([\d]+), Y\+([\d]+)\nPrize: X=([\d]+), Y=([\d]+)$"
machines = list(map(lambda x: tuple(map(int, re.fullmatch(r, x).groups())), input))
print(f"Part 1: {sum(map(play, machines))}")
print(f"Part 2: {sum(map(lambda x: play(x, True), machines))}")