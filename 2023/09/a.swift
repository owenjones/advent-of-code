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
  differences.reverse()

  for i in 0...(differences.count - 2) {
    differences[i + 1].append((differences[i].last! + differences[i + 1].last!))
  }

  extrapolated.append(differences.last!.last!)
}

let sum = extrapolated.reduce(0, +)
print("Sum of extrapolated values: \(sum)")