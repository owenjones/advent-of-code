import java.io.File
import kotlin.system.exitProcess

var changes = File("input.txt").bufferedReader().readLines()

var i = 0
var total = 0
var previous = mutableListOf<Int>()

while(true) {
  total += changes[i].toInt()
  
  if (previous.contains(total)) {
    println("First frequency we reach twice is ${total}")
    exitProcess(0)
  }

  previous.add(total)
  i = (i + 1) % changes.size
}
