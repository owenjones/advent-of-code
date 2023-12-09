import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ $0.split(separator: " ").map{ Int($0)! } }

var extrapolated: [Int] = []
for sequence in input {
  var differences: [[Int]] = []
  differences.append(sequence)
  
  while differences.last!.reduce(0, +) != 0 {
    let dv = zip(differences.last!, differences.last![1...]).map{ ($0.1 - $0.0) }
    differences.append(dv)
  }

  var reversed = differences.reversed().map{ Array($0.reversed()) }
  for i in 0...(reversed.count - 2) {
    reversed[i + 1].append((reversed[i + 1].last! - reversed[i].last!))
  }

  extrapolated.append(reversed.last!.last!)
}

let sum = extrapolated.reduce(0, +)
print("Sum of extrapolated values: \(sum)")