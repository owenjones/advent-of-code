import Foundation
let input = try String(contentsOfFile: "input.txt").split(separator: "\n")

var total = 0
for str in input {
  var numbers = str.components(separatedBy: CharacterSet.decimalDigits.inverted)
  numbers.removeAll{ $0 == "" }
  total += Int("\(numbers.first!.first!)\(numbers.last!.last!)")!
}

print("Sum of calibration values: \(total)") // 55816