import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")
let game = #/Game (\d+): (.*)/#
let dice = #/(\d+) (red|green|blue)/#

var total = 0
for line in input {
  if let match = line.wholeMatch(of: game) {
    var count: [String: Int] = [:]
    let pulled = String(match.2)

    let numbers = pulled.replacingOccurrences(of: ";", with: ",").split(separator: ",").map{$0.trimmingCharacters(in: .whitespaces)}
    for number in numbers {
      if let pulled = number.wholeMatch(of: dice) {
        count[String(pulled.2)] = max((count[String(pulled.2)] ?? 0), (Int(pulled.1) ?? 0))
      }
    }

    let power = (count["red"] ?? 0) * (count["green"] ?? 0) * (count["blue"] ?? 0)
    total = total + power
  }
}
print("Sum of cube powers: \(total)") // 63307