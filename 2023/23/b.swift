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

struct Segment {
  let start: C
  let finish: C
  let length: Int
  let children: [Segment]
}

let input = try String(contentsOfFile: "test.txt")
let map = Map(tiles: input)

let segments: [Segment] = []

func segmentSearch(_ c: C) -> [Segment] {
  var previous: C? = nil
  var current = c

  while true {
    var possible: [C] = []
    for d in Direction.allCases {
      let n = current + d.vec()
      if map.inBounds(n) && map.get(n) != "#" && n != previous {
        possible.append(n)
      }
    }

    if possible.count == 0 {
      // dead end - drop the segment
    }
    else if possible.count > 1 {
      // reached an intersection, close up the current segment and
      // recursively start search in possible directions
      print("reached intersection at (\(current.x), \(current.y))")
      return []
    }
    else {
      // continue along the current line
      previous = current
      current = possible[0]
    }
  }
}

segmentSearch(C(0, 0))

// 6242 = too low
// 6298 = wrong...