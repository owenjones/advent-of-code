def countQuestions(response)
    response = response.strip.split
    number = response.size
    counts = Array.new(26) { |x| 0 }
    response.join.each_char.map { |c| counts[c.ord - 97] += 1}

    total = 0
    counts.each.each do |c| 
        if c == number 
            total += 1 
        end
    end
    return total
end

groups = Array.new

File.open("input.txt") do |input|
    groups = input.read.split("\n\n")
end

counts = groups.map { |g| countQuestions(g) }
puts "Sum of all counts: #{counts.sum}"