import Foundation

class Module {
  var id: String
  var type: String
  var outputs: [String]

  var state: Bool = false

  var inputs: [String] = []
  var last: [String: Bool] = [:]

  init(_ input: String) {
    if let match = input.wholeMatch(of: #/([%&]{1})?([a-z]+) -> ([a-z, ]+)/#) {
      id = String(match.2)
      outputs = match.3.split(separator: ", ").map{ String($0) }
      type = String(match.1 ?? "B")
    }
    else {
      print(input)
      fatalError()
    }
  }

  func process(_ pulse: (String, String, Bool), system: System) -> [(String, String, Bool)] {
    switch type {
      case "B":
        return outputs.map{ (id, $0, pulse.2) }

      case "%":
        if !pulse.2 {
          state.toggle()
          return outputs.map{ (id, $0, state) }
        }
        else {
          return []
        }

      case "&":
        var value = false
        if inputs.count > 1 {
          last[pulse.0] = pulse.2
          value = !(inputs.map{ last[$0, default: false] }.filter{ $0 == true }.count == inputs.count)
        }
        else {
          value = !pulse.2
        }
        
        return outputs.map{ (id, $0, value) }

      default:
        fatalError()
    }
  }

}

class System {
  var presses: Int = 0
  var high: Int = 0
  var low: Int = 0

  var modules: [String: Module] = [:]
  var flipflops: [Module] = []
  var queue: [(String, String, Bool)] = []

  init(_ input: String) {
    let definitions = input.split(separator: "\n")
    for module in definitions {
      let m = Module(String(module))
      modules[m.id] = m
      if m.type == "%" { flipflops.append(m) }
    }

    for (id, module) in modules {
      for c in module.outputs {
        if modules[c] != nil { modules[c]!.inputs.append(id) }
      }
    }
  }

  func button(from: String = "broadcaster") {
    presses += 1
    queue.append(("", from, false))
    while queue.count > 0 {
      let pulse = queue.removeFirst()
      if pulse.2 { high += 1 } else { low += 1 }
      if modules[pulse.1] != nil {
        queue.append(contentsOf: modules[pulse.1]!.process(pulse, system: self))
      }
    }
  }
}

func gcd(_ l: Int, _ r: Int) -> Int {
  return (r == 0) ? l : gcd(r, (l % r))
}

func lcm(_ steps: [Int]) -> Int {
  return steps.reduce(1, { a, b in (b * a) / gcd(b, a) })
}

// ---------------------------------------------------

let input = try String(contentsOfFile: "input.txt")
let sys = System(input)

var presses = Array(repeating: 0, count: sys.modules["broadcaster"]!.outputs.count)
for (i, module) in sys.modules["broadcaster"]!.outputs.enumerated() {
  while true {
    presses[i] += 1
    sys.button(from: module)
    if sys.flipflops.map({ sys.modules[$0.id]!.state }).filter({ $0 == false }).count == sys.flipflops.count { break }
  }
}

let answer = lcm(presses)
print("rx will be low after \(answer) presses")