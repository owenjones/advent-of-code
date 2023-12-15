import Foundation

extension Array where Element == String {
  mutating func transpose() {
    var transposed: [String] = Array(repeating: "", count: self.first!.count)
    for l in self {
      for (i, e) in l.enumerated() {
        transposed[i].append(e)
      }
    }
    self = transposed
  }

  mutating func reverse() {
    self = self.map{ String($0.reversed()) }
  }

  mutating func rotate(_ deg: Int) {
    switch deg {
      case 90:
        transpose()
        reverse()
      case 180:
        transpose()
        reverse()
        transpose()
        reverse()
      case -90:
        reverse()
        transpose()
      case -180:
        transpose()
        reverse()
        transpose()
        reverse()
      default:
        return
    }
  }
}

class RockPlatform {
  var rocks: [String]

  init(_ rocks: [String]) {
    self.rocks = rocks
  }

  func clockwise() {
    rocks.rotate(90)
  }

  func anticlockwise() {
    rocks.rotate(-90)
  }

  func tilt() {
    // always tilt east (lines of rocks sort that way by default)
    rocks = rocks.map{ $0.split(separator: "#", omittingEmptySubsequences: false).map{ String($0.sorted()) }.joined(separator: "#") }
  }

  func cycle() {
    for _ in 1...4 {
      tilt()
      clockwise()
    }
  }

  func display() {
    print(rocks.joined(separator: "\n"))
    print()
  }

  func load() -> Int {
    let load = rocks.enumerated().map { ($0.element.filter{ $0 == "O" }.count) * (rocks.count - $0.offset) }
    return load.reduce(0, +)
  }
}

let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ String($0) }
var platform = RockPlatform(input)
platform.clockwise()

var i = 0
var final = false
var cache: [String: Int] = [:]
while i < 1000000000 {
  platform.cycle()

  let current = platform.rocks.joined()
  if !final, let previous = cache[current] {
    let period = i - previous
    final = true
    i = 1000000000 - ((1000000000 - i) % period)
  }
  else {
    cache[current] = i
  }

  i += 1
}

platform.anticlockwise()
print("Load on north support beams: \(platform.load())")