import Foundation

extension Array where Element == String {
  mutating func transpose() {
    var transposed: [String] = Array(repeating: "", count: self.first!.count)
    for l in self {
      for (i, e) in l.enumerated() {
        transposed[i].append(e)
      }
    }
    self = transposed
  }
}

func findReflection(_ mirror: [String]) -> Int? {
  let n = mirror.count
  for i in 0..<(n - 1) {
    if mirror[i] == mirror[i + 1] {
      let extend = min(i, (n - 1) - (i + 1))
      for j in 0...extend {
        if mirror[i - j] != mirror[i + 1 + j] {
          break
        }

        if j == extend {
          return i + 1
        }
      }
    }
  }
  return nil
}

let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n").map{ $0.split(separator: "\n").map{ String($0) } }
var summary = 0
for original in input {
  if let horizontal = findReflection(original) {
    summary += horizontal * 100
  }
  else {
    var mirror = original
    mirror.transpose()
    summary += findReflection(mirror) ?? 0
  }
}
print("Summary of all mirrors: \(summary)")