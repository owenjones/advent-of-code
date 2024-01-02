import Foundation

struct C: Hashable {
  let x: Int
  let y: Int
  let steps: Int

  init(_ x: Int, _ y: Int, _ steps: Int) {
    self.x = x
    self.y = y
    self.steps = steps
  }

  func hash(into hasher: inout Hasher) {
    hasher.combine(x)
    hasher.combine(y)
  }

  static func + (_ lhs: C, _ rhs: C) -> C {
    return C((lhs.x + rhs.x), (lhs.y + rhs.y), (lhs.steps + rhs.steps))
  }
}

enum Direction: CaseIterable {
  case N, E, S, W

  func vec() -> C {
    switch self {
      case .N:
        return C(0, -1, 1)
      case .E:
        return C(1, 0, 1)
      case .S:
        return C(0, 1, 1)
      case .W:
        return C(-1, 0, 1)
    }
  }
}

struct Map {
  let tiles: [[String]]
  let width: Int
  let height: Int

  init(_ tiles: String) {
    self.tiles = tiles.split(separator: "\n").map{ $0.split(separator: "").map{ String($0) } }
    self.width = self.tiles.first!.count
    self.height = self.tiles.count
  }

  func get(_ c: C) -> String {
    return tiles[c.y][c.x]
  }

  func find(_ s: String) -> C? {
    for i in 0..<width {
      for j in 0..<height {
        if tiles[j][i] == s { return C(i, j, 0) }
      }
    }

    return nil
  }

  func inBounds(_ c: C) -> Bool {
    return c.x >= 0 && c.x < width && c.y >= 0 && c.y < height
  }
}

// ------------------------

let input = try String(contentsOfFile: "input.txt")
let map = Map(input)
let start = map.find("S")!

var visited: [C: Bool] = [:]
var possible: [C: Bool] = [:]
var queue: [C] = [start]
while queue.count > 0 {
  let current = queue.removeFirst()

  for d in Direction.allCases {
    let n = current + d.vec()
    if map.inBounds(n) && map.get(n) != "#" && !visited[C(n.x, n.y, 0), default: false] && n.steps <= 64 {
      if n.steps % 2 == 0 { possible[C(n.x, n.y, 0)] = true }
      visited[C(n.x, n.y, 0)] = true
      queue.append(n)
    }
  }

}

let answer = possible.count
print("Possible to visit \(answer) gardens in exactly 64 steps")