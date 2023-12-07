import Foundation

extension Int {
  init?(fromCard: String) {
    switch fromCard {
      case "A":
        self.init(14)
      case "K":
        self.init(13)
      case "Q":
        self.init(12)
      case "J":
        self.init(11)
      case "T":
        self.init(10)
      default:
        self.init(fromCard)
    }
  }
}

enum HandType: Int {
  case HighCard, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind

  static func < (lhs: Self, rhs: Self) -> Bool {
    return lhs.rawValue < rhs.rawValue
  }

  static func > (lhs: Self, rhs: Self) -> Bool {
    return lhs.rawValue > rhs.rawValue
  }
}

class Hand: Comparable {
  var raw: String
  var bid: Int
  var cards: [Int]
  var type: HandType
  var description: String { return "Hand <\(type): \(raw))>" }

  init(_ input: Substring) {
    let s = input.split(separator: " ")
    bid = Int(s[1]) ?? 0
    raw = String(s[0])
    cards = s[0].split(separator: "").map{ Int(fromCard: String($0)) ?? 0 }

    let mapped = cards.map{ ($0, 1) }
    let counts = Dictionary(mapped, uniquingKeysWith: +).values
    let high = counts.max()
    
    switch high {
      case 5:
        type = HandType.FiveOfAKind
      case 4:
        type = HandType.FourOfAKind
      case 3:
        if counts.count == 2 {
          type = HandType.FullHouse
        }
        else {
          type = HandType.ThreeOfAKind
        }
      case 2:
        if counts.count == 3 {
          type = HandType.TwoPair
        }
        else {
          type = HandType.OnePair
        }
      default:
        type = HandType.HighCard
    }
  }

  static func == (lhs: Hand, rhs: Hand) -> Bool {
    return lhs.type == rhs.type && lhs.cards == rhs.cards
  }

  static func > (lhs: Hand, rhs: Hand) -> Bool {
    if lhs.type == rhs.type {
      for i in (0...4) {
        if lhs.cards[i] == rhs.cards[i] {
          continue
        }
        
        return lhs.cards[i] > rhs.cards[i]
      }

      return false
    }
    else {
      return lhs.type > rhs.type
    }
  }

  static func < (lhs: Hand, rhs: Hand) -> Bool {
    if lhs.type == rhs.type {
      for i in (0...4) {
        if lhs.cards[i] == rhs.cards[i] {
          continue
        }

        return lhs.cards[i] < rhs.cards[i]
      }

      return false
    }
    else {
      return lhs.type < rhs.type
    }
  }
}

let input = try String(contentsOfFile: "input.txt").split(separator: "\n").map { Hand($0) }
let winnings = input.sorted().enumerated().map{ ($0.offset + 1) * $0.element.bid }.reduce(0, +)

print("Total winnings: \(winnings)")