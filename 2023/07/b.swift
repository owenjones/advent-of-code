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
      case "T":
        self.init(10)
      case "J":
        self.init(1)
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
    let counts = Dictionary(mapped, uniquingKeysWith: +)
    let matches = counts.filter({ $0.key != 1 }).values
    let jokers = counts[1] ?? 0
    
    switch matches.max() {
      case 5:
        type = HandType.FiveOfAKind

      case 4:
        type = (jokers == 1) ? HandType.FiveOfAKind : HandType.FourOfAKind

      case 3:
        switch jokers {
          case 2:
            type = HandType.FiveOfAKind

          case 1:
            type = HandType.FourOfAKind

          default:
            if matches.count == 2 {
              type = HandType.FullHouse
            }
            else {
              type = HandType.ThreeOfAKind
            }
        }

      case 2:
        switch jokers {
          case 3:
            type = HandType.FiveOfAKind

          case 2:
            type = HandType.FourOfAKind

          case 1:
            if matches.count == 2 {
              type = HandType.FullHouse
            }
            else {
              type = HandType.ThreeOfAKind
            }

          default:
            if matches.count == 3 {
              type = HandType.TwoPair
            }
            else {
              type = HandType.OnePair
            }
        }
        
      case 1:
        switch jokers {
          case 4:
            type = HandType.FiveOfAKind
          case 3:
            type = HandType.FourOfAKind
          case 2:
            type = HandType.ThreeOfAKind
          case 1:
            type = HandType.OnePair
          default:
            type = HandType.HighCard
        }

      default:
          if jokers == 5 {
            type = HandType.FiveOfAKind
          }
          else {
            type = HandType.HighCard
          }
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