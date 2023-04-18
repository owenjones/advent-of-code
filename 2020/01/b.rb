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


lines = Array.new

File.open("input.txt") do |input|
  lines = input.read.split
end

lines = lines.map { |line| Integer(line) }

answer = solve(lines)
puts "Expense report: #{answer}"