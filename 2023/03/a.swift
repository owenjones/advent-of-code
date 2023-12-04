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

let digits = try NSRegularExpression(pattern: #"(\d+)"#)

var specialChars = CharacterSet()
specialChars.insert(charactersIn: #"!@#$%^&*()_-+=[]{};:'"|\/?<>,`~"#)

// for each real line (ignoring the pad lines) in the input
for i in (1...(input.count - 1)) {
  // find range of any numbers in the line
  let ranges = digits.matches(in: input[i], range: NSRange(input[i].startIndex..., in: input[i]))
  for range in ranges {
    // for each range, extract the number and all characters around it
    let extended = Range(NSMakeRange((range.range.location - 1), (range.range.length + 2)), in: input[i])!
    let chars = [input[i-1][extended], input[i][extended], input[i+1][extended]].joined()

    // test if the digit + surrounding characters contain any special characters
    if chars.rangeOfCharacter(from: specialChars) != nil {
      sum = sum + (Int(input[i][Range(range.range(at: 1), in: input[i])!]) ?? 0)
    }
  }
}

print("Sum of engine parts: \(sum)") // 536202