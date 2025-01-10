def countQuestions(response)
    response = response.strip.split
    number = response.size
    counts = Array.new(26) { |x| 0 }
    response.join.each_char.map { |c| counts[c.ord - 97] += 1}
    return counts.select { |c| c == number }.size
end

groups = File.open("input.txt").read.split("\n\n")
counts = groups.map { |g| countQuestions(g) }
puts "Sum of all counts: #{counts.sum}"