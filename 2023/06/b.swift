import Foundation
let input = try String(contentsOfFile: "input.txt")

var ways = 0
if let matches = input.wholeMatch(of: #/Time: (.*)\nDistance: (.*)/#) {
  let time = Int(String(matches.1).split(separator: " ").joined())!
  let distance = Int(String(matches.2).split(separator: " ").joined())!

  var beating = 0
  for i in (1...(time-1)) {
    let travelled = i * (time - i)
    if travelled > distance {
      beating += 1
    }
  }
  if beating > 0 {
    ways += beating
  }
}

print("Ways to beat the race: \(ways)")