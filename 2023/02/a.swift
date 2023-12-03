import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")

var total = 0
for line in input {
  if let match = line.wholeMatch(of: #/Game (\d+): (.*)/#) {
    var possible = true
    let pulls = match.2.split(separator: ";").map{$0.trimmingCharacters(in: .whitespaces)}
    
    for hand in pulls {
      var count: [String: Int] = [:]
      let numbers = hand.split(separator: ",").map{$0.trimmingCharacters(in: .whitespaces)}
      for number in numbers {
        if let pulled = number.wholeMatch(of: #/(\d+) (red|green|blue)/#) {
          count[String(pulled.2)] = (count[String(pulled.2)] ?? 0) + (Int(pulled.1) ?? 0)
        }
      }

      if((count["red"] ?? 0) > 12 || (count["green"] ?? 0) > 13 || (count["blue"] ?? 0) > 14) {
        possible = false
      }
    }

    if(possible) {
      total = total + (Int(match.1) ?? 0)
    }
  }
}

print("Sum of IDs from possible games: \(total)") // 2416