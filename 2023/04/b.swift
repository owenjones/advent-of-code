import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")

var multiplier: [Int: Int] = [:]
for line in input {
  if let match = line.wholeMatch(of: #/Card ([\w ]+): ([\w ]*) \| ([\w ]*)/#) {
    let id = Int(match.1.trimmingCharacters(in: .whitespaces)) ?? 0
    let winning = Set(match.2.split(separator: " ").map{Int($0.trimmingCharacters(in: .whitespaces)) ?? 0})
    let drawn = Set(match.3.split(separator: " ").map{Int($0.trimmingCharacters(in: .whitespaces)) ?? 0})
    let matching = drawn.intersection(winning).count
    
    multiplier[id] = multiplier[id] ?? 1
    if matching > 0 {
      for i in (id+1)...(id+matching) {
        multiplier[i] = multiplier[i] ?? 1
        multiplier[i]! += multiplier[id] ?? 1
      }
    }
  }
}

let total = (1...input.count).map{ multiplier[$0]! }.reduce(0, +)
print("Scratchcards: \(total)") // 23806951