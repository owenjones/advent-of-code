import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map({ $0.split(separator: "") })

var round: [[Int]] = Array(repeating: [], count: input[0].count)
var cube: [[Int]] = Array(repeating: [101], count: input[0].count)
for (x, line) in input.enumerated() {
  for (y, char) in line.enumerated() {
    switch char {
      case "O":
        round[y].append(100 - x)

      case "#":
        cube[y].append(100 - x)

      default:
        continue
    }
  }
}

var load = 0
for (y, l) in round.enumerated() {
  for x in l {
    let biggest = cube[y].filter({ $0 > x }).min()! - 1
    cube[y].append(biggest)
    load += biggest
  }
}
print("Load on north support beams: \(load)")