import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")

var total = 0
for line in input {
  if let match = line.wholeMatch(of: #/Card ([\w ]+): ([\w ]*) \| ([\w ]*)/#) {
    let winning = Set(match.2.split(separator: " ").map{$0.trimmingCharacters(in: .whitespaces)}.map{(Int($0) ?? 0)})
    let drawn = Set(match.3.split(separator: " ").map{$0.trimmingCharacters(in: .whitespaces)}.map{(Int($0) ?? 0)})
    let matching = drawn.intersection(winning).count
    total += Int(pow(Double(2), Double(matching - 1)))
  }
}

print("Scratchcards total points: \(total)") // 20407