import Foundation

func countArrangements(pattern: String, groups: [Int], depth: Int) -> Int {
  if pattern.isEmpty {
    return (groups.isEmpty && depth == 0) ? 1 : 0
  }

  var count = 0
  if ["#", "?"].contains(pattern[pattern.startIndex]) {
    count += countArrangements(pattern: String(pattern.dropFirst()), groups: groups, depth: depth + 1)
  }
  if [".", "?"].contains(pattern[pattern.startIndex]) && (!groups.isEmpty && groups[0] == depth || depth == 0) {
    count += countArrangements(pattern: String(pattern.dropFirst()), groups: (depth > 0) ? Array(groups.dropFirst()) : groups, depth: 0)
  }
  return count
}

let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ 
  let s = $0.split(separator: " ") 
  return (String(s[0] + "."), s[1].split(separator: ",").map{ Int($0)! })
}

let counts = input.map{ countArrangements(pattern: $0.0, groups: $0.1, depth: 0) }.reduce(0, +)
print("Sum of possible arrangements: \(counts)")