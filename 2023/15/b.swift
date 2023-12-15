import Foundation

func HASH(_ input: String) -> Int {
  var value = 0
  input.split(separator: "").forEach{
    let c = Character(String($0)).asciiValue ?? 0
    value += Int(c)
    value *= 17
    value %= 256
  }
  return value
}

let steps = try String(contentsOfFile: "input.txt").split(separator: ",")
var boxes: [[(String, Int)]] = Array(repeating: [], count: 256)

steps.forEach{
  if let matches = String($0).wholeMatch(of: #/([A-Za-z]+)([-=]{1})([0-9]?)/#) {
    let label = HASH(String(matches.1))
    let index = boxes[label].enumerated().compactMap{ $0.element.0 == String(matches.1) ? $0.offset : nil }
    if matches.2 == "=" {
      let lens = (String(matches.1), Int(matches.3)!)
      if !index.isEmpty {
        for i in index { 
          boxes[label].remove(at: i)
          boxes[label].insert(lens, at: i)
        }
      }
      else {
        boxes[label].append(lens)
      }
    }
    else if matches.2 == "-" && !index.isEmpty {
      for i in index { 
        boxes[label].remove(at: i)
      }
    }
  }
}

var power = 0
for (i, box) in boxes.enumerated() {
  for (j, lens) in box.enumerated() {
    power += (i + 1) * (j + 1) * lens.1
  }
}
print("Focussing power: \(power)")