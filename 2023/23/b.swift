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
    var tiles: [[String]]
    let width: Int
    let height: Int

    init(tiles: String) {
        self.tiles = tiles.split(separator: "\n").map{ $0.split(separator: "").map{ String($0) } }
        self.width = self.tiles.first!.count
        self.height = self.tiles.count
    }

    mutating func update(x: Int, y: Int, value: String) {
        tiles[y][x] = value
    }

    func get(_ c: C) -> String {
        return inBounds(c) ? tiles[c.y][c.x] : ""
    }

    func inBounds(_ c: C) -> Bool {
        return c.x >= 0 && c.x < width && c.y >= 0 && c.y < height
    }
}

func parse(map: Map) {
    for y in map.tiles {
        for x in y {
            
        }
    }
}

// ----------------------------

let input = try String(contentsOfFile: "test.txt", encoding: String.Encoding.ascii)
var map = Map(tiles: input)
map.update(x: 1, y: 0, value: "#")
map.update(x: map.width - 2, y: map.height - 1, value: "#")

parse(map: map)

// 6242 = too low
// 6298 = wrong...