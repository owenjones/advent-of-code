import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n")
let directions = input[0].split(separator: "")
var nodes: [String: (String, String)] = [:]

input[1].split(separator: "\n").forEach{
  if let matches = $0.wholeMatch(of: #/([\w]{3}) = \(([\w]{3}), ([\w]{3})\)/#) {
    nodes[String(matches.1)] = (String(matches.2), String(matches.3))
  }
}

let starts = nodes.keys.filter({ $0.wholeMatch(of: #/([\w]{2})A/#) != nil })
let steps: [Int] = starts.map{
  var steps = 0
  var node = $0
  while true {
    if node.wholeMatch(of: #/([\w]{2})Z/#) != nil {
      break
    }

    let next = directions[(steps % directions.count)]
    node = (next == "L") ? nodes[node]!.0 : nodes[node]!.1
    steps += 1
  }
  return steps
}

func gcd(_ l: Int, _ r: Int) -> Int {
  return (r == 0) ? l : gcd(r, (l % r))
}

func lcm(_ steps: [Int]) -> Int {
  return steps.reduce(1, { a, b in (b * a) / gcd(b, a) })
}

print("Number of steps to reach end of all starts: \(lcm(steps))")