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

func getSegment(_ c: C, _ p: [C] = []) -> (C, Int, [C])? {
  var previous: [C] = p
  var current = c
  var length: Int = 0

  while true {
    print(current)
    var possible: [C] = []
    for d in Direction.allCases {
      let n = current + d.vec()
      if map.inBounds(n) && map.get(n) != "#" && !previous.contains(n) {
        possible.append(n)
      }
    }

    if possible.count == 0 {
      if current == C(map.width - 2, map.height - 1) {
        // edge case to make sure we don't miss the end
        return (current, length, [])
      }
      return nil
    }
    else if possible.count > 1 {
      // reached an intersection, close up the current segment and
      // recursively start search in possible directions
      return (current, length, possible)
    }
    else {
      // continue along the current line
      previous.append(current)
      current = possible[0]
      length += 1
    }
  }
}

// ----------------------------

let input = try String(contentsOfFile: "test.txt")
let map = Map(tiles: input)

var start = C(1, 0)
var ends: [C] = []
var beginning: C, length: Int, next: [C]
while true {
  let segment = getSegment(start, ends)

  if segment != nil {
    beginning = segment!.0
    length = segment!.1
    next = segment!.2

    print(beginning)
    print(next)
    print()

    start = next[0]
    ends.append(contentsOf: next)
    if next.count == 0 { break } 
  }
  break
}


let segments: [Segment] = []


// 6242 = too low
// 6298 = wrong...