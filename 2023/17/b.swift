import Foundation

struct Block {
  let map: [[Int]]

  func get(_ c: C) -> Int {
    return map[c.x][c.y]
  } 

  func valid(_ c: C) -> Bool {
    return c.x >= 0 && c.x < map.first!.count && c.y >= 0 && c.y < map.count
  }
}

struct C: Equatable, Hashable {
  let x: Int
  let y: Int

  init(_ x: Int, _ y: Int) {
    self.x = x
    self.y = y
  }

  static func + (_ lhs: Self, _ rhs: Self) -> Self {
    return C(lhs.x + rhs.x, lhs.y + rhs.y)
  }

  static func == (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.x == rhs.x && lhs.y == rhs.y
  }

  func hash(into hasher: inout Hasher) {
    hasher.combine(x)
    hasher.combine(y)
  }
}

struct Node: Comparable {
  let head: C
  let direction: C
  let path: [C]
  let steps: Int
  let loss: Int
  let straight: Int

  init(_ head: C, direction: C, path: [C], steps: Int = 0, loss: Int = 0, straight: Int = 0) {
    self.head = head
    self.direction = direction
    self.path = path
    self.steps = steps
    self.loss = loss
    self.straight = straight
  }

  func cost() -> Int {
    return self.loss - (self.head.x + self.head.y + self.steps)
  }

  static func == (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.cost() == rhs.cost()
  }

  static func > (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.cost() > rhs.cost()
  }

  static func < (_ lhs: Self, _ rhs: Self) -> Bool {
    return lhs.cost() < rhs.cost()
  }
}

let input = try String(contentsOfFile: "test.txt").split(separator: "\n").map{ $0.split(separator: "").map{ Int($0) ?? 0 } }
let map = Block(map: input)

let target = C((input.first!.count - 1), (input.count - 1))
let start = C(0, 0)
let node = Node(start, direction: C(0, 1), path: [start])

var open: [Node] = [node]
var closed: [C:Node] = [:]
var lowest: [Int] = []

while true {
  let current = open.removeFirst()

  if current.head == target && current.straight > 3 { 
    print("Least heat loss that can occur: \(current.loss)")
    break
  }

  for direction in [C(0, 1), C(0, -1), C(-1, 0), C(1, 0)] {
    if(direction == C(-current.direction.x, -current.direction.y) || (direction == current.direction && current.straight >= 10) || (direction != current.direction && current.straight < 4)) {
      continue
    }

    print("heading: (\(direction.x), \(direction.y))")

    let h = current.head + direction
    if map.valid(h) {
      var p = current.path
      p.append(h)

      let n = Node(
        h, 
        direction: direction, 
        path: p,
        steps: current.steps + 1,
        loss: current.loss + map.get(h), 
        straight: (direction == current.direction) ? (current.straight + 1) : 0
        )

      if closed[h] == nil || (closed[h] != nil && closed[h]!.cost() > n.cost()) {
        open.append(n)
        closed[h] = n
      }
    } 
  }

  open.sort()
}