import Foundation
let map = try String(contentsOfFile: "input.txt").split(separator: "\n").map { $0.split(separator: "").map{ String($0) } }

/*
coordinates in (x, y)

(0,0) -> extending y
|
v extending x
*/

let directions = [
  // ((input), (output)) - assuming clockwise flow
  "|": ((-1, 0), (1, 0)),
  "-": ((0, -1), (0, 1)),
  "L": ((-1, 0), (0, 1)),
  "J": ((-1, 0), (0, -1)),
  "7": ((0, -1), (1, 0)),
  "F": ((0, 1), (1, 0)),
  ]

// TODO: make allllll of this much nicer...

// Step 1: find the start
var start: (Int, Int) = (0, 0)
for x in map.enumerated() {
  for y in x.element.enumerated() {
    if y.element == "S" {
      start = (x.offset, y.offset)
    }
  }
}

// Step 2: find the two valid pipes next to the start
var nodes: [(Int, Int)] = []
let cardinals = [
  (-1, 0, ["|", "7", "F"]),
  (0, 1, ["-", "7", "J"]),
  (1, 0, ["|", "L", "J"]),
  (0, -1, ["-", "L", "F"])
  ]
for c in cardinals {
  let x = start.0 + c.0
  let y = start.1 + c.1
  if x >= 0 && y >= 0 && c.2.contains(map[x][y]) {
    nodes.append((x, y))
  }
}

// Step 3: keep stepping along pipe pieces in both clockwise and counter-clockwise directions until we meet
var last = (start, start)
var steps = 1
while nodes[0] != nodes[1] {
  let next = (directions[map[nodes[0].0][nodes[0].1]]!, directions[map[nodes[1].0][nodes[1].1]]!)
  let cw = [(nodes[0].0 + next.0.0.0, nodes[0].1 + next.0.0.1), (nodes[0].0 + next.0.1.0, nodes[0].1 + next.0.1.1)].filter({ $0 != last.0 })[0]
  let ccw = [(nodes[1].0 + next.1.0.0, nodes[1].1 + next.1.0.1), (nodes[1].0 + next.1.1.0, nodes[1].1 + next.1.1.1)].filter({ $0 != last.1 })[0]
  
  last = (nodes[0], nodes[1])
  nodes = [cw, ccw]
  steps += 1
}

print("Steps to get to the furthest pipe piece: \(steps)")