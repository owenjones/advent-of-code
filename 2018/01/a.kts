import java.io.File

var total = 0

File("input.txt").forEachLine {
  total += it.toInt()
}

println("Total is $total")