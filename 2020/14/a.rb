memory = Hash.new { 0 }

File.open("test_input.txt") do |input|

end

total = memory.sum { |k,v| v }
puts "Sum of all values in memory: #{total}"