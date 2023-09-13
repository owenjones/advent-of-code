import java.io.File

val ids = File("input.txt").bufferedReader().readLines().sorted().map({ id -> id.toCharArray() })

val p: (Boolean) -> Boolean = { it == false }
for (i in 0..(ids.size - 2)) {
  var pairs = (ids[i] zip ids[i+1])
  
  val matching = pairs.map({ (a, b) -> a == b })
  if (matching.count(p) == 1) {
    val letters = matching.indices.filter({ matching[it] == true }).map({ it -> pairs[it].first }).joinToString("")
    println("Common letters: ${letters}") // fonbwmjquwtapeyzikghtvdxl
  }
}
