import Foundation
let rawInput = try String(contentsOfFile: "input.txt").split(separator: "\n")

// pad the puzzle input with a period in each direction
// to avoid having to do any bounds checking later
let pad = String(repeating: ".", count: (rawInput[0].count + 2))
var input = [pad]
for line in rawInput {
  var s = String(line)
  s.insert(".", at: s.startIndex)
  s.insert(".", at: s.endIndex)
  input.append(s)
}
input.append(pad)

var sum = 0

let asterisk = try NSRegularExpression(pattern: #"([*]{1})"#)
let digits = try NSRegularExpression(pattern: #"(\d+)"#)

// for each real line (ignoring the pad lines) in the input
for i in (1...(input.count - 1)) {
  // find position of any asterisks in the line
  let ranges = asterisk.matches(in: input[i], range: NSRange(input[i].startIndex..., in: input[i]))
  for range in ranges {
    // for each asterisk, extract an 8x3 segment around it
    let extended = Range(NSMakeRange((range.range.location - 4), (range.range.length + 8)), in: input[i])!
    let chars = [input[i-1][extended], input[i][extended], input[i+1][extended]]

    var parts: [Int] = []
    for segment in chars {
      let line = String(segment)
      let numbers = digits.matches(in: line, range: NSRange(line.startIndex..., in: line))
      for n in numbers {
        // test if range of digit touches/intersects position 4 (the asterisk)
        let start = n.range.location
        let end = start + n.range.length - 1

        if(start == 5 || end == 3 || start <= 4 && end >= 4) {
          parts.append(Int(line[Range(n.range(at: 1), in: line)!])!)
        }
      }
    }
    if(parts.count == 2) {
      sum = sum + (parts[0] * parts[1])
    }
  }
}

print("Sum of gear ratios: \(sum)") // 78272573