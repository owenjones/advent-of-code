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

func characterDiff(_ l: String, _ r: String) -> Int {
  let matches = zip(l, r).map{ $0.0 == $0.1 }
  let count = matches.filter({ $0 == false }).count
  return count
}

func findSmudgedReflection(_ mirror: [String]) -> Int? {
  let n = mirror.count
  for i in 0..<(n - 1) {
    var differences = 0
    let extend = min(i, (n - 1) - (i + 1))
    for j in 0...extend {
      if mirror[i - j] != mirror[i + 1 + j] {
        if differences < 1 {
          differences += characterDiff(mirror[i - j], mirror[i + 1 + j])
          if differences > 1 { break }
        }
        else { break }
      }

      if j == extend && differences == 1 {
        return i + 1
      }
    }
  }
  return nil
}

let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n").map{ $0.split(separator: "\n").map{ String($0) } }
var summary = 0
for original in input {
  if let horizontal = findSmudgedReflection(original) {
    summary += horizontal * 100
  }
  else {
    var mirror = original
    mirror.transpose()
    summary += findSmudgedReflection(mirror) ?? 0
  }
}
print("Summary of all mirrors: \(summary)")