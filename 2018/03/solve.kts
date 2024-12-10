import java.io.File
import kotlin.text.Regex

data class Pattern(val id: Int, val t: Pair<Int, Int>, val b: Pair<Int, Int>)
val regex = Regex("""^#(\d+) @ (\d+),(\d+): (\d+)x(\d+)$""")

fun splitPattern(pattern: String): Pattern {
  val match = regex.find(pattern)!!.destructured.toList().map({ it.toInt() })
  return Pattern(
    match[0],
    Pair(match[1], match[2]),
    Pair(match[1] + match[3] - 1, match[2] + match[4] - 1)
  )
}

val patterns = File("input.txt").readLines().map{ splitPattern(it) }
var fabric = Array(1000) { Array(1000) { 0 } }

for (pattern in patterns) {
  for (x in pattern.t.first..pattern.b.first) {
    for (y in pattern.t.second..pattern.b.second) {
      fabric[y][x] += 1
    }
  }
}

val claimed = fabric.flatten().filter{ it >= 2 }.count()
println(claimed)

var single: MutableSet<Pair<Int, Int>> = mutableSetOf()
for (x in 0..999) {
  for (y in 0..999) {
    if(fabric[y][x] == 1) {
      single.add(Pair(x, y))
    }
  }
}

fun testPattern(pattern: Pattern): Boolean {
  for (x in pattern.t.first..pattern.b.first) {
    for (y in pattern.t.second..pattern.b.second) {
      if (!single.contains(Pair(x, y))) {
        return false
      }
    }
  }

  println(pattern.id)
  return true
}

for (p in patterns) {
  if (testPattern(p)) {
    break
  }
}
