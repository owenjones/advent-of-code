import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ String($0) }

struct Step {
  let direction: String
  let steps: Int
}

var steps: [Step] = []
for line in input {
  if let match = line.wholeMatch(of: #/(\w){1} (\d*) \(#(.*)\)/#) {
    steps.append(Step(direction: String(match.1), steps: Int(match.2)!))
  }
}

var area = 0
var l = (0, 0)
for step in steps {
  var x: Int
  var y: Int

  switch step.direction {
    case "U": (x, y) = (l.0, l.1 - step.steps)
    case "D": (x, y) = (l.0, l.1 + step.steps)
    case "L": (x, y) = (l.0 - step.steps, l.1)
    case "R": (x, y) = (l.0 + step.steps, l.1)
    default: continue
  }

  area += (l.1 + y) * (l.0 - x)
  area += step.steps
  l = (x, y)
}

area = (abs(area) / 2) + 1
print("Total area: \(area)")