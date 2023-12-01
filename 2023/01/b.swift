import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")
let regex = try NSRegularExpression(pattern: #"(?=(\d{1}|one|two|three|four|five|six|seven|eight|nine))"#)

let replacements = [
  "one": "1", "two": "2", "three": "3", "four": "4", "five": "5",
  "six": "6", "seven": "7", "eight": "8", "nine": "9"
]

var total = 0
for line in input {
  let str = String(line)
  let matches = regex.matches(in: str, range: NSRange(str.startIndex..., in: str))
  let numbers = matches.compactMap{ Range($0.range(at: 1), in: str) }.map{ str[$0] }
  var value = String(numbers.first! + numbers.last!)
  for (k, v) in replacements {
    value = value.replacingOccurrences(of: k, with: v)
  }
  total += Int(value)!
}

print("Sum of calibration values: \(total)") // 54980