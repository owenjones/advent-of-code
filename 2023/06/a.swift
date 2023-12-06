import Foundation
let input = try String(contentsOfFile: "input.txt")

var ways: [Int] = []
if let matches = input.wholeMatch(of: #/Time: (.*)\nDistance: (.*)/#) {
  let times = String(matches.1).split(separator: " ").map{ Int($0)! }
  let distances = String(matches.2).split(separator: " ").map{ Int($0)! }
  let races = zip(times, distances)

  for race in races {
    var beating = 0
    for i in (1...(race.0 - 1)) {
      let distance = i * (race.0 - i)
      if distance > race.1 {
        beating += 1
      }
    }
    if beating > 0 {
      ways.append(beating)
    }
  }
}

let answer = ways.reduce(1, *)
print("Product of number of ways to win each race: \(answer)")