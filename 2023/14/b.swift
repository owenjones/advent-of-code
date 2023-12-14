import Foundation
let input = try String(contentsOfFile: "test.txt").split(separator: "\n").map({ $0.split(separator: "") })

let DIM = 10
let CYCLES = 1000000000

enum Direction: Int {
  case None, N, W, S, E
  var next: Direction {
    switch self {
      case .N:
        return .W
      case .W:
        return .S
      case .S:
        return .E
      case .E:
        return .N
      default:
        return .None
    }
  }
}

struct Coord: Comparable, Hashable {
  let x: Int
  let y: Int
  let d: Direction

  static func == (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.d == rhs.d
  }

  static func < (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.x < rhs.x && lhs.y < rhs.y
  }

  static func > (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.x > rhs.x && lhs.y > rhs.y
  }

  func hash(into hasher: inout Hasher) {
    hasher.combine(x)
    hasher.combine(y)
    hasher.combine(d.rawValue)
  }
}

var round: Set<Coord> = []
var cubeX: [[Int]] = Array(repeating: [], count: input.count + 1)
var cubeY: [[Int]] = Array(repeating: [], count: input[0].count)
for (x, line) in input.enumerated() {
  for (y, char) in line.enumerated() {
    switch char {
      case "O":
        round.insert(Coord(x: DIM - x, y: y, d: Direction.N))

      case "#":
        cubeX[DIM - x].append(y)
        cubeY[y].append(DIM - x)

      default:
        continue
    }
  }
}

var states: Set<Set<Coord>> = []
var dir: Direction = .N
var i = 0
while i < 10 {
  // print(i)
  // print(round)
  // print(round.count)
  // print(dir)
  if states.contains(round) {
    // i = CYCLES - (CYCLES/i)
    print("cache hit") 
  }
  
  var next: Set<Coord> = []
  let nd = dir.next
  switch dir {
    case .N:
      var filled = cubeY
      for c in round {
        let biggest = (filled[c.y].filter({ $0 > c.x }).min() ?? DIM + 1) - 1
        filled[c.y].append(biggest)
        next.insert(Coord(x: biggest, y: c.y, d: nd))
      }
      
    case .W:
      var filled = cubeX
      for c in round {
        let smallest = (filled[c.x].filter({ $0 < c.y }).min() ?? -1) + 1
        filled[c.x].append(smallest)
        next.insert(Coord(x: c.x, y: smallest, d: nd))
      }

    case .S:
      var filled = cubeY
      for c in round {
        let smallest = (filled[c.y].filter({ $0 < c.x }).min() ?? 0) + 1
        filled[c.y].append(smallest)
        next.insert(Coord(x: smallest, y: c.y, d: nd))
      }

    case .E:
      var filled = cubeX
      for c in round {
        let biggest = (filled[c.x].filter({ $0 > c.y }).min() ?? DIM + 1) - 1
        filled[c.x].append(biggest)
        next.insert(Coord(x: c.x, y: biggest, d: nd))
      }

    default:
      print("OH NO")
  }

  round = next
  states.insert(round)
  print(states.count)
  dir = nd
  i += 1
}

let load = round.map{ $0.x }.reduce(0, +)
print("Load on north support beams: \(load)")