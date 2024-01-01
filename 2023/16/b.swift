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

struct Beam {
  var c: C
  var d: Direction
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

  func inBounds(_ c: C) -> Bool {
    return c.x >= 0 && c.x < width && c.y >= 0 && c.y < height
  }
}

let input = try String(contentsOfFile: "input.txt")
let map = Map(input)
var totals: [Int] = []

var starts: [(C, Direction)] = []
for d in Direction.allCases {
  switch d {
    case .N:
      for i in 0..<map.width {
        starts.append((C(i, map.height), d))
      }

    case .E:
      for i in 0..<map.height {
        starts.append((C(-1, i), d))
      }

    case .S:
      for i in 0..<map.width {
        starts.append((C(i, -1), d))
      }

    case .W:
      for i in 0..<map.height {
        starts.append((C(map.width, i), d))
      }
  }
}

for start in starts {
  var beams: [Beam] = [Beam(c: start.0, d: start.1)]
  var energised: [C: Int] = [:]
  var visited: [Direction: [C: Bool]] = [Direction.N: [:], Direction.E: [:], Direction.S: [:], Direction.W: [:]]

  while beams.count > 0 {
    var beam = beams.removeFirst()
    let n = beam.c + beam.d.vec()
    let seen = visited[beam.d]![n, default: false]

    if map.inBounds(n) && !seen {
      energised[n] = 1
      visited[beam.d]![n] = true

      switch (map.get(n), beam.d) {
        case (".", _), ("-", Direction.E), ("-", Direction.W), ("|", Direction.N), ("|", Direction.S):
          beam.c = n
          beams.append(beam)

        case ("/", Direction.N):
          beam.c = n
          beam.d = Direction.E
          beams.append(beam)

        case ("/", Direction.E):
          beam.c = n
          beam.d = Direction.N
          beams.append(beam)

        case ("/", Direction.S):
          beam.c = n
          beam.d = Direction.W
          beams.append(beam)

        case ("/", Direction.W):
          beam.c = n
          beam.d = Direction.S
          beams.append(beam)

        case (#"\"#, Direction.N):
          beam.c = n
          beam.d = Direction.W
          beams.append(beam)

        case (#"\"#, Direction.E):
          beam.c = n
          beam.d = Direction.S
          beams.append(beam)

        case (#"\"#, Direction.S):
          beam.c = n
          beam.d = Direction.E
          beams.append(beam)

        case (#"\"#, Direction.W):
          beam.c = n
          beam.d = Direction.N
          beams.append(beam)

        case ("|", Direction.E), ("|", Direction.W):
          beam.c = n
          beam.d = Direction.N
          beams.append(beam)
          beams.append(Beam(c: n, d: Direction.S))

        case ("-", Direction.N), ("-", Direction.S):
          beam.c = n
          beam.d = Direction.E
          beams.append(beam)
          beams.append(Beam(c: n, d: Direction.W))

        default:
          break
      }
    }
  }

  let count = energised.values.reduce(0, +)
  totals.append(count)
}

print("Total possible energised tiles: \(totals.max()!)")
