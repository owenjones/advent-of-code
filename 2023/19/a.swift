import Foundation

struct Part {
  let r: [String : Int]

  init(_ part: String) {
    if let match = part.wholeMatch(of: #/\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\}/#) {
      r = ["x": Int(match.1)!, "m": Int(match.2)!, "a": Int(match.3)!, "s": Int(match.4)!]
    }
    else {
      r = ["x": 0, "m": 0, "a": 0, "s": 0]
    }
  }

  func rating() -> Int {
    return r.values.reduce(0, +)
  }
}

class Workflow {
  let id: String
  var rules: [(String, String, Int, String)] = []

  init(_ rule: String) {
    if let match = rule.wholeMatch(of: #/([a-z]+){(.*)}/#) {
      id = String(match.1)
      let rStrings = String(match.2).split(separator: ",").map{ String($0) }
      rules = rStrings.map{ expandRule($0) }
      // print(rules)
    }
    else {
      id = "X"
      rules = []
    }
  }

  func expandRule(_ s: String) -> (String, String, Int, String) {
    let r = s.split(separator: ":")
    if r.count > 1 {
      let t = r[0].wholeMatch(of: #/([xmas]{1})([><]{1})(\d+)/#)
      return (String(t!.1), String(t!.2), Int(t!.3)!, String(r[1]))
    }
    else {
      return ("X", "X", 0, String(r[0]))
    }
  }

  func process(_ p: Part, workflows: [String: Workflow]) -> Bool {
    for r in rules {
      if r.0 == "X" {
        switch r.3 {
          case "A": return true
          case "R": return false
          default: return workflows[r.3]!.process(p, workflows: workflows)
        }
      }
      else {
        if (r.1 == ">") ? p.r[r.0]! > r.2 : p.r[r.0]! < r.2 { 
          switch r.3 {
            case "A": return true
            case "R": return false
            default: return workflows[r.3]!.process(p, workflows: workflows)
          }
        }
      }
    }
    return false
  }
}

var workflows: [String: Workflow] = [:]
let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n").map{ $0.split(separator: "\n") }
for w in input[0] { 
  let w = Workflow(String(w))
  workflows[w.id] = w
}

var accepted: [Int] = []
for p in input[1] {
  let p = Part(String(p))
  let accept = workflows["in"]!.process(p, workflows: workflows)
  if accept { accepted.append(p.rating()) }
}

let answer = accepted.reduce(0, +)
print("Sum of ratings of accepted parts: \(answer)")