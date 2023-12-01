import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")
let regex = try NSRegularExpression(pattern: #"(?=(\d{1}))"#)

var total = 0
for line in input {
  let str = String(line)
  let matches = regex.matches(in: str, range: NSRange(str.startIndex..., in: str))
  let numbers = matches.compactMap{ Range($0.range(at: 1), in: str) }.map{ str[$0] }
  total += Int(numbers.first! + numbers.last!)!
}

print("Sum of calibration values: \(total)") // 55816