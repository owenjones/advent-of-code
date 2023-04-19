def countQuestions(response)
    response = response.strip().gsub(/[\r\n]/, "")
    counts = Array.new(26) { |x| 0 }
    response.each_char.map { |c| counts[c.ord - 97] = 1}
    return counts.sum
end

groups = Array.new

File.open("input.txt") do |input|
    groups = input.read.split("\n\n")
end

counts = groups.map { |g| countQuestions(g) }
puts "Sum of all counts: #{counts.sum}"