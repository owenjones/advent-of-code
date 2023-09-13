import java.io.File

fun enumerateLetters(id: String): MutableList<Int> {
  var counts = MutableList(26) { 0 }
  id.toCharArray().map({c -> (c.toInt() - 97)}).forEach { counts[it]++ }
  return counts
}

val ids = File("input.txt").bufferedReader().readLines()

var twos = 0
var threes = 0

for (box in ids) {
  var counts = enumerateLetters(box)
  if (counts.contains(2)) twos++
  if (counts.contains(3)) threes++
}

var checksum = twos * threes
println("Checksum is ${checksum}") // 4920