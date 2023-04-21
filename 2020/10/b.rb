numbers = Array.new
File.open("input.txt") do |input|
  numbers = input.read.split.map { |x| Integer(x) }
end

rating = numbers.max + 3
numbers = numbers.sort
numbers.insert(0, 0)
numbers.push(rating)

counts = Hash.new(0)
counts[0] = 1

numbers.each do |n|
  next if n == 0
  counts[n] = counts[n - 1] + counts[n - 2] + counts[n - 3]
end

puts "Total possible combinations: #{counts[numbers.last]}"