numbers = File.open("input.txt").read.split.map { |x| Integer(x) }.sort
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