import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")

let replacements = [
  "one": "o1e", "two": "t2o", "three": "t3e", "four": "f4r", "five": "f5e",
  "six": "s6x", "seven": "s7n", "eight": "e8t", "nine": "n9e"
  ] // yes I know this is super hacky but they overlap and cba to do it better

var total = 0
for str in input {
  var m = String(str)
  for (k, v) in replacements {
    m = m.replacingOccurrences(of: k, with: v)
  }

  var numbers = m.components(separatedBy: CharacterSet.decimalDigits.inverted)
  numbers.removeAll{ $0 == "" }
  total += Int("\(numbers.first!.first!)\(numbers.last!.last!)")!
}

print("Sum of calibration values: \(total)") // 54980