import Foundation

class Piece: Comparable {
  var start: Double
  var finish: Double
  var offset: Double
  var description: String { return "Piece <\(start), \(finish), \(offset)>" }

  init(start: Double = -Double.infinity, finish: Double = Double.infinity, offset: Double = 0) {
    self.start = start
    self.finish = finish
    self.offset = offset
  }

  func inRange(_ x: Double) -> Bool {
    return x >= start && x < finish
  }

  func transform(_ x: Double) -> Double {
    return x + offset
  }

  static func composite(_ l: Piece, _ r: Piece) -> Piece? {
    let s = r.start - l.offset
    let f = r.finish - l.offset

    if l.finish < s || l.start > f {
      return nil
    }

    let start = max(l.start, s)
    let finish = min(l.finish, f)
    let offset = l.offset + r.offset

    return Piece(start: start, finish: finish, offset: offset)
  }

  static func == (_ l: Piece, _ r: Piece) -> Bool {
    return (l.start == r.start) && (l.finish == r.finish) && (l.offset == r.offset)
  }

  static func < (_ l: Piece, _ r: Piece) -> Bool {
    return l.start < r.start
  }

  static func > (_ l: Piece, _ r: Piece) -> Bool {
    return l.start > r.start
  }
}

class PiecewiseFunction {
  var pieces: [Piece] = []

  init(_ pieces: [Piece]) {
    self.pieces = (pieces.count > 0) ? complete(pieces) : []
  }

  func complete(_ pieces: [Piece]) -> [Piece] {
    let sorted = pieces.sorted()
    var extended: [Piece] = []
    
    if sorted.first!.start != -Double.infinity {
      extended.append(Piece(finish: sorted.first!.start))
    }

    for piece in sorted {
      if extended.count > 0 && extended.last!.finish < piece.start {
        extended.append(Piece(start: extended.last!.finish, finish: piece.start))
      }

      extended.append(piece)
    }

    if extended.last!.finish != Double.infinity {
      extended.append(Piece(start: extended.last!.finish))
    }

    return extended
  }

  static func composite(_ l: PiecewiseFunction?, _ r: PiecewiseFunction?) -> PiecewiseFunction? {
    if l == nil {
      return r
    }

    var pieces: [Piece] = []
    for x in l!.pieces {
      for y in r!.pieces {
        if let composite = Piece.composite(x, y) {
          pieces.append(composite)
        }
      }
    }

    return PiecewiseFunction(pieces)
  }

  func transform(_ x: Double) -> Double? {
    for piece in pieces {
      if piece.inRange(x) {
        return piece.transform(x)
      }
    }

    return nil
  }
}

let input = try String(contentsOfFile: "input.txt").split(separator: "\n\n")

var functions: [PiecewiseFunction] = []
for map in input[1...] {
  let pieces = map.split(separator: "\n").suffix(from: 1).map{
    let n = $0.split(separator: " ").map{ Double(String($0))! }
    return Piece(start: n[1], finish: (n[1] + n[2]), offset: (n[0] - n[1]))
  }

  functions.append(PiecewiseFunction(pieces))
}
let optimusPrime = functions.reduce(nil, { a, b in PiecewiseFunction.composite(a, b) })

let seeds = input[0].wholeMatch(of: #/seeds: (.*)/#)!.1.split(separator: " ").map{ Int($0) ?? 0 }
let locations = seeds.map{ Int(optimusPrime?.transform(Double($0)) ?? 0) }
let lowest = locations.min() ?? 0
print("Lowest location corresponding to an initial seed: \(lowest)")
