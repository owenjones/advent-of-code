numbers = File.open("input.txt").read.split.map { |x| Integer(x) }.sort
counts = Hash.new(0)
counts[0] = 1

numbers.each do |n|
  counts[n] = counts[n - 1] + counts[n - 2] + counts[n - 3]
end

puts "Total possible combinations: #{counts[numbers.last]}"