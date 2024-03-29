import Foundation

var input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ String($0) }

// Expand universe
let allColumns = Set(0..<input[0].count)
let filledColumns = Set(input.flatMap{ $0.enumerated().compactMap{ $0.element == "#" ? $0.offset : nil } })
let emptyColumns = allColumns.symmetricDifference(filledColumns).sorted()
let emptyRows = input.enumerated().compactMap{ ($0.element.contains("#") == false) ? $0.offset : nil }

for (i, r) in emptyRows.enumerated() {
  input.insert(String(repeating: ".", count: input[0].count), at: r + i)
}

var universe: [[String]] = []
for r in input.map({ $0.split(separator: "") }) {
  var n = r
  for (i, c) in emptyColumns.enumerated() {
    n.insert(".", at: c + i)
  }

  universe.append(n.map{ String($0) })
}

// Find coordinates of galaxies
var galaxies: [(Int, Int)] = []
for (x, r) in universe.enumerated() {
  for (y, c) in r.enumerated() {
    if c == "#" {
      galaxies.append((x, y))
    }
  }
}

// Pair up all the galaxies
var paired: [((Int, Int), (Int, Int))] = []
for (i, a) in galaxies.enumerated() {
  for (j, b) in galaxies.enumerated() {
    if a == b || j < i {
      continue
    }

    paired.append((a, b))
  }
}

// Get all lengths and sum
let lengths = paired.map{ abs($0.0.0 - $0.1.0) + abs($0.0.1 - $0.1.1) }
let sum = lengths.reduce(0, +)
print("Sum of the shortest paths between each galaxy: \(sum)")