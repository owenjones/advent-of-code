import Foundation

func graph(_ file: String) -> [String: [String]] {
  var adjacent: [String: [String]] = [:]
  if let input = try? String(contentsOfFile: file) {
    for line in input.split(separator: "\n") {
      let split = line.split(separator: ":")
      let left = String(split[0])

      for right in split[1].split(separator: " ").map({ String($0) }) {
        if adjacent[left] == nil { adjacent[left] = [] }
        if adjacent[right] == nil { adjacent[right] = [] }

        adjacent[left]!.append(right)
        adjacent[right]!.append(left)
      }
    }
  }
  return adjacent
}

func KargersAlgorithm(_ original: [String: [String]]) -> (Int, Int, Int) {
  var graph = original
  var sizes: [String: Int] = [:]

  func updateEdges(_ old: String, _ new: String) { 
    if graph[old] != nil {
      for (_, n) in graph[old]!.enumerated() {
        while true {
          let i = graph[n]!.firstIndex(of: old)
          if i != nil {
            graph[n]!.remove(at: i!)
            graph[n]!.append(new)
          }
          else {
            break
          }
        }
      }
    }
  }
  
  var i = 0;
  while graph.count > 2 {
    let x = graph.keys.randomElement()!
    let y = graph[x]!.randomElement()!

    let nx = graph[x]!.filter({ $0 != y })
    let ny = graph[y]!.filter({ $0 != x })
    graph[String(i)] = nx + ny
    updateEdges(x, String(i))
    updateEdges(y, String(i))

    sizes[String(i)] = sizes[x, default: 1] + sizes[y, default: 1]

    graph.removeValue(forKey: x)
    graph.removeValue(forKey: y)
    i += 1
  }

  let vertices = Array(graph.keys)
  let first = vertices.first!
  let last = vertices.last!
  return (graph[first]!.count, sizes[first, default: 1], sizes[last, default: 1])
}

let g = graph("input.txt")
var (size, l, r) = KargersAlgorithm(g)
while size != 3 {
  (size, l, r) = KargersAlgorithm(g)
}

let answer = l * r
print("Product of two partitions: \(answer)")

