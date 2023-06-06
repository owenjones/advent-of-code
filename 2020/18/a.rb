def solve(sum)
  
end

lines = File.open("test_input.txt").read.split("\n")
answer = lines.map { |l| solve(l) }
puts "Answers: #{answer.to_s}"
puts "Sum of values: #{answer.sum}"

# Test set answers: 71, 51, 26, 437, 12240, 13632 (sum is 26457)