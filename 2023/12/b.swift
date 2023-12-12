import Foundation

struct HashableInput: Hashable {
  let pattern: String
  let groups: [Int]
  let depth: Int

  static func == (lhs: Self, rhs: Self) -> Bool {
    return (lhs.pattern == rhs.pattern && lhs.groups == rhs.groups && lhs.depth == rhs.depth)
  }

  func hash(into hasher: inout Hasher) {
    hasher.combine(pattern)
    hasher.combine(groups)
    hasher.combine(depth)
  }
}

func cache<Input: Hashable, Output>(_ function: @escaping (Input) -> Output) -> (Input) -> Output {
  var cache = [Input: Output]()

  return { input in
    if let cached = cache[input] {
      return cached
    }

    let result = function(input)
    cache[input] = result
    return result
  }
}

func countArrangements(_ i: HashableInput) -> Int {
  if i.pattern.isEmpty {
    return (i.groups.isEmpty && i.depth == 0) ? 1 : 0
  }

  var count = 0
  if ["#", "?"].contains(i.pattern[i.pattern.startIndex]) {
    count += cached(HashableInput(pattern: String(i.pattern.dropFirst()), groups: i.groups, depth: i.depth + 1))
  }
  if [".", "?"].contains(i.pattern[i.pattern.startIndex]) && (!i.groups.isEmpty && i.groups[0] == i.depth || i.depth == 0) {
    count += cached(HashableInput(pattern: String(i.pattern.dropFirst()), groups: (i.depth > 0) ? Array(i.groups.dropFirst()) : i.groups, depth: 0))
  }
  return count
}

let cached = cache(countArrangements)

let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map{ 
  let s = $0.split(separator: " ") 
  let pattern = Array(repeating: s[0], count: 5).joined(separator: "?")
  let groups = s[1].split(separator: ",").map{ Int($0)! }
  return (String(pattern + "."), Array(Array(repeating: groups, count: 5).joined()))
}

let counts = input.map{ cached(HashableInput(pattern: $0.0, groups: $0.1, depth: 0)) }.reduce(0, +)
print("Sum of possible arrangements: \(counts)")