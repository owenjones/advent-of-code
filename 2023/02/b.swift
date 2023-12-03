import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")

var total = 0
for line in input {
  if let match = line.wholeMatch(of: #/Game (\d+): (.*)/#) {
    var count: [String: Int] = [:]
    let dice = String(match.2)
    let numbers = dice.replacingOccurrences(of: ";", with: ",").split(separator: ",").map{$0.trimmingCharacters(in: .whitespaces)}
    for number in numbers {
      if let pulled = number.wholeMatch(of: #/(\d+) (red|green|blue)/#) {
        count[String(pulled.2)] = max((count[String(pulled.2)] ?? 0), (Int(pulled.1) ?? 0))
      }
    }
    total = total + (count["red"] ?? 0) * (count["green"] ?? 0) * (count["blue"] ?? 0)
  }
}

print("Sum of cube powers: \(total)") // 63307