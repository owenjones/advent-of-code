import Foundation

struct Part {
  let r: [String : ClosedRange<Int>]

  init(x: ClosedRange<Int>, m: ClosedRange<Int>, a: ClosedRange<Int>, s: ClosedRange<Int>) {
    r = ["x": x, "m": m, "a": a, "s": s]
  }

  func count() -> Int {
    return r.values.map{ $0.upperBound - $0.lowerBound + 1 }.reduce(1, *)
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
    }
    else {
      id = "X"
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

  func process(_ p: Part) -> ([(String, Part)], [Part]) {
    var c = p
    var open: [(String, Part)] = []
    var close: [Part] = []

    for r in rules {
      if r.0 == "X" {
        switch r.3 {
          case "A":
            close.append(c)
            break
          case "R":
            break
          default: 
            open.append((r.3, c))
            break
        }
      }
      else {
        let a, b: Part
        switch r.0 { // split part into lower/upper ranges - possibly a cleaner way to do this??
          case "x":
            if r.1 == "<" {
              a = Part(x: c.r["x"]!.lowerBound...(r.2 - 1), m: c.r["m"]!, a: c.r["a"]!, s: c.r["s"]!)
              b = Part(x: r.2...c.r["x"]!.upperBound, m: c.r["m"]!, a: c.r["a"]!, s: c.r["s"]!)
            }
            else {
              b = Part(x: c.r["x"]!.lowerBound...r.2, m: c.r["m"]!, a: c.r["a"]!, s: c.r["s"]!)
              a = Part(x: (r.2 + 1)...c.r["x"]!.upperBound, m: c.r["m"]!, a: c.r["a"]!, s: c.r["s"]!)
            }
            

          case "m":
            if r.1 == "<" {
              a = Part(x: c.r["x"]!, m: c.r["m"]!.lowerBound...(r.2 - 1), a: c.r["a"]!, s: c.r["s"]!)
              b = Part(x: c.r["x"]!, m: r.2...c.r["m"]!.upperBound, a: c.r["a"]!, s: c.r["s"]!)
            }
            else {
              b = Part(x: c.r["x"]!, m: c.r["m"]!.lowerBound...r.2, a: c.r["a"]!, s: c.r["s"]!)
              a = Part(x: c.r["x"]!, m: (r.2 + 1)...c.r["m"]!.upperBound, a: c.r["a"]!, s: c.r["s"]!)
            }

          case "a":
            if r.1 == "<" {
              a = Part(x: c.r["x"]!, m: c.r["m"]!, a: c.r["a"]!.lowerBound...(r.2 - 1), s: c.r["s"]!)
              b = Part(x: c.r["x"]!, m: c.r["m"]!, a: r.2...c.r["a"]!.upperBound, s: c.r["s"]!)
            }
            else {
              b = Part(x: c.r["x"]!, m: c.r["m"]!, a: c.r["a"]!.lowerBound...r.2, s: c.r["s"]!)
              a = Part(x: c.r["x"]!, m: c.r["m"]!, a: (r.2 + 1)...c.r["a"]!.upperBound, s: c.r["s"]!)
            }

          case "s":
            if r.1 == "<" {
              a = Part(x: c.r["x"]!, m: c.r["m"]!, a: c.r["a"]!, s: c.r["s"]!.lowerBound...(r.2 - 1))
              b = Part(x: c.r["x"]!, m: c.r["m"]!, a: c.r["a"]!, s: r.2...c.r["s"]!.upperBound)
            }
            else {
              b = Part(x: c.r["x"]!, m: c.r["m"]!, a: c.r["a"]!, s: c.r["s"]!.lowerBound...r.2)
              a = Part(x: c.r["x"]!, m: c.r["m"]!, a: c.r["a"]!, s: (r.2 + 1)...c.r["s"]!.upperBound)
            }
            

          default: fatalError()
        }

        c = b // keep range which didn't match the rule to pass on to next

        switch r.3 {
          case "A":
            close.append(a)
          case "R":
            break
          default: 
            open.append((r.3, a))
        }
      }
    }

    return (open, close)
  }
}

// ----------------------------------------

var workflows: [String: Workflow] = [:]
let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n").map{ $0.split(separator: "\n") }
for w in input[0] { 
  let w = Workflow(String(w))
  workflows[w.id] = w
}

var open: [(String, Part)] = [("in", Part(x: 1...4000, m: 1...4000, a: 1...4000, s: 1...4000))]
var closed: [Part] = []

while open.count > 0 {
  let check = open.removeFirst()
  let (toOpen, toClose) = workflows[check.0]!.process(check.1)
  open.append(contentsOf: toOpen)
  closed.append(contentsOf: toClose)
}

let answer = closed.map{ $0.count() }.reduce(0, +)
print("Total number of acceptable parts: \(answer)")
