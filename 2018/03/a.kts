import java.io.File

fun splitPattern(pattern) {

}

val patterns = File("test_input.txt").bufferedReader().readLines()
var fabric = Array(1000) { IntArray(1000) { 0 } }

// for each pattern work out x..x+dx and y..y+dy, +1 to all fabric indices that includes
for (pattern in patterns) {
  val dx = 0
}