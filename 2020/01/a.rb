def solve(lines)
  lines.each do |a|
    lines.each do |b|
      if a + b == 2020
        return a * b
      end
    end
  end
end


lines = File.open("input.txt").read.split.map { |line| Integer(line) }
answer = solve(lines)
puts "Expense report: #{answer}"