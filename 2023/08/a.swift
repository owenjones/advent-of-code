import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n")
let directions = input[0].split(separator: "")
var nodes: [String: (String, String)] = [:]

input[1].split(separator: "\n").forEach{
  if let matches = $0.wholeMatch(of: #/([A-Z]{3}) = \(([A-Z]{3}), ([A-Z]{3})\)/#) {
    nodes[String(matches.1)] = (String(matches.2), String(matches.3))
  }
}

var steps = 0
var node = "AAA"
while true {
  if node == "ZZZ" {
    break
  }

  let next = directions[(steps % directions.count)]
  node = (next == "L") ? nodes[node]!.0 : nodes[node]!.1
  steps += 1
}

print("Number of steps to reach end: \(steps)")

