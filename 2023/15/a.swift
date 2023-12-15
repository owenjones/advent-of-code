import Foundation

func HASH(_ input: String) -> Int {
  var value = 0
  input.forEach{
    let c = Character(String($0)).asciiValue ?? 0
    value += Int(c)
    value *= 17
    value %= 256
  }
  return value
}

let steps = try String(contentsOfFile: "input.txt").split(separator: ",")
let results = steps.map{ HASH(String($0)) }.reduce(0, +)
print("Sum of all results: \(results)")