import Foundation

struct C: Hashable {
  let x: Int
  let y: Int

  init(_ x: Int, _ y: Int) {
    self.x = x
    self.y = y
  }

  func hash(into hasher: inout Hasher) {
    hasher.combine(x)
    hasher.combine(y)
  }

  static func manhattanDistance(_ lhs: C, _ rhs: C) -> Int {
    return abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y)
  }

  static func + (_ lhs: C, _ rhs: C) -> C {
    return C((lhs.x + rhs.x), (lhs.y + rhs.y))
  }
}

enum Direction: CaseIterable {
  case N, E, S, W

  func vec() -> C {
    switch self {
      case .N:
        return C(0, -1)
      case .E:
        return C(1, 0)
      case .S:
        return C(0, 1)
      case .W:
        return C(-1, 0)
    }
  }
}

struct Map {
  let tiles: [[String]]
  let width: Int
  let height: Int

  init(tiles: String) {
    self.tiles = tiles.split(separator: "\n").map{ $0.split(separator: "").map{ String($0) } }
    self.width = self.tiles.first!.count
    self.height = self.tiles.count
  }

  func get(_ c: C) -> String {
    return tiles[c.y][c.x]
  }

  func inBounds(_ c: C) -> Bool {
    return c.x >= 0 && c.x < width && c.y >= 0 && c.y < height
  }
}

struct Frontier: Hashable, Comparable {
  let position: C
  let steps: Int
  let distance: Int
  let path: [C]

  func hash(into hasher: inout Hasher) {
    hasher.combine(position)
    hasher.combine(steps)
    hasher.combine(path)
  }

  static func == (_ lhs: Frontier, _ rhs: Frontier) -> Bool {
    return lhs.steps == rhs.steps && lhs.distance == rhs.distance
  }

  static func > (_ lhs: Frontier, _ rhs: Frontier) -> Bool {
    return lhs.steps > rhs.steps || lhs.distance > rhs.distance
  }

  static func < (_ lhs: Frontier, _ rhs: Frontier) -> Bool {
    return lhs.steps < rhs.steps || lhs.distance < rhs.distance
  }
}

let input = try String(contentsOfFile: "input.txt")
let map = Map(tiles: input)
let start = C(1, 0)
let finish = C((map.width - 2), (map.height - 1))

var open: [Frontier] = [Frontier(
  position: start,
  steps: 0,
  distance: C.manhattanDistance(start, finish),
  path: []
  )]
var closed: [C: (Int, Int)] = [:]

var lengths: [Int] = []

while open.count > 0 {
  let current = open.removeFirst()

  if(current.position == finish) {
    print(current.steps)
    lengths.append(current.steps)
    continue
  }

  for d in Direction.allCases {
    let c = current.position + d.vec()
    let distance = C.manhattanDistance(c, finish)

    if map.inBounds(c) && map.get(c) != "#" && !current.path.contains(c) {
      if closed[c] == nil || (closed[c] != nil && (closed[c]!.0 < (current.steps + 1) || closed[c]!.1 <= distance)) {
        var p = current.path
        p.append(c)

        let f = Frontier(
          position: c,
          steps: current.steps + 1,
          distance: distance,
          path: p
        )

        closed[c] = (f.steps, f.distance)
        open.append(f)
      }
    }
  }

  open.sort(by: >)
}

let longest = lengths.max() ?? 0
print("Longest path: \(longest)")
print(lengths)

// 6242 = too low
// 6298 = wrong...