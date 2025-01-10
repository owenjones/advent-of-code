def solve(lines)
  lines.each do |a|
    lines.each do |b|
      lines.each do |c|
        if a + b + c == 2020
          return a * b * c
        end
      end
    end
  end
end


lines = File.open("input.txt").read.split.map { |line| Integer(line) }
answer = solve(lines)
puts "Expense report: #{answer}"