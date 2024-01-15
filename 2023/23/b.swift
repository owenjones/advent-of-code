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

struct M: Hashable {
  let s: C
  let e: C
  let d: Direction

  init(_ s: C, _ e: C, _ d: Direction) {
    self.s = s
    self.e = e
    self.d = d
  }

  func hash(into hasher: inout Hasher) {
    hasher.combine(s)
    hasher.combine(e)
    hasher.combine(d)
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

class Segment {
  let start: C
  var finish: C? = nil
  var length: Int = 0
  var children: [Segment] = []
  var visited: [C: Bool] = [:]

  init(start: C) {
    self.start = start
  }

  func longestRouteFrom() -> Int {
    if children.count == 0 { return length }
    let lengths = children.map { $0.longestRouteFrom() }
    return length + lengths.max()! + 1 // + 1 as we need to step from this segment to the next
  }
}

func getSegments(_ map: Map, _ start: C) -> Segment {
  let beginning = Segment(start: start)
  var incomplete: [Segment] = [beginning]

  func segmentSearch(start: C, visited: [C: Bool]) -> (C?, Int?, [M]?, [C: Bool]?) {
    var length = 0
    var current: C = start
    var visited = visited

    while true {
      // print(current)
      visited[current] = true
      var possible: [M] = []
      for d in Direction.allCases {
        let n = current + d.vec()
        if map.inBounds(n) && map.get(n) != "#" && !visited[n, default: false] {
          possible.append(M(start, n, d))
        }
      }

      if possible.count == 0 {
        if current == C(map.width - 2, map.height - 1) {
          // edge case to make sure we don't miss the end
          return (current, length, [], visited)
        }

        return (nil, nil, nil, nil)
      }
      else if possible.count > 1 {
        // reached an intersection, close up the current segment and
        // recursively start search in possible directions
        return (current, length, possible, visited)
      }
      else {
        // continue along the current line
        current = possible[0].e
        length += 1
      }
    }
  }

  var complete: [M: Bool] = [:]
  while incomplete.count > 0 {
    let s = incomplete.removeFirst()
    let (end, length, next, visited) = segmentSearch(start: s.start, visited: s.visited)
    if end == nil { continue }
    s.finish = end!
    s.length = length!
    s.visited.merge(visited!) { (current, _) in current }

    for child in next! {
      if complete[child, default: false] { continue }
      complete[child] = true
      let n = Segment(start: child.e)
      n.visited = s.visited
      s.children.append(n)
      incomplete.append(n)
    }
  }

  return beginning
}

// ----------------------------

let input = try String(contentsOfFile: "test.txt")
let map = Map(tiles: input)
let start = C(1, 0)
let route = getSegments(map, start)
let longest = route.longestRouteFrom()
print("Longest possible hike: \(longest)")

// 6242 = too low
// 6298 = wrong...