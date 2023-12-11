import Foundation

var input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ String($0) }

// Find the empty rows/columns (where expansion would happen)
let allColumns = Set(0..<input[0].count)
let filledColumns = Set(input.flatMap{ $0.enumerated().compactMap{ $0.element == "#" ? $0.offset : nil } })
let emptyColumns = allColumns.symmetricDifference(filledColumns).sorted()
let emptyRows = input.enumerated().compactMap{ ($0.element.contains("#") == false) ? $0.offset : nil }

// Find coordinates of galaxies
var galaxies: [(Int, Int)] = []
for (x, r) in input.enumerated() {
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

// Get all lengths and sum -> if we cross an empty row/column add on the length
// an expansion would have resulted in
let expand = 1000000
var sum = 0
for pair in paired {
  let manhattan = abs(pair.0.0 - pair.1.0) + abs(pair.0.1 - pair.1.1)
  let x = (pair.0.0 < pair.1.0) ? (pair.0.0...pair.1.0) : (pair.1.0...pair.0.0)
  let y = (pair.0.1 < pair.1.1) ? (pair.0.1...pair.1.1) : (pair.1.1...pair.0.1)
  let rows = emptyRows.filter({ x.contains($0) }).count
  let columns = emptyColumns.filter({ y.contains($0) }).count
  sum += manhattan + ((rows + columns) * (expand - 1))
}
print("Sum of the shortest paths between each galaxy: \(sum)")