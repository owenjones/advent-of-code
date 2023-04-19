numbers = Array.new
File.open("test_input.txt") do |input|
  numbers = input.read.split("\n").map { |x| Integer(x) }
end
