numbers = Array.new
File.open("input.txt") do |input|
  numbers = input.read.split.map { |x| Integer(x) }
end

numbers = numbers.sort
numbers.insert(0, 0)
numbers.push(numbers.max + 3)

ones = 0
threes = 0
numbers.each_cons(2) do |n|
  diff = n[1] - n[0]
  if diff == 1
    ones += 1
  elsif diff == 3
    threes += 1
  end
end
difference = ones * threes

puts "Total jolt differences: #{difference}"