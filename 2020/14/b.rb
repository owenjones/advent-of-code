def decodeLocations(mask, coded)
  mask = mask.split("")
  bits = value.to_s(2)
  padded = (("0" * (mask.size - bits.size)) + bits).split("")

  
end

input = Array.new
File.open("test_input.txt") { |i| input = i.read.split("\n") }

memory = Hash.new { 0 }
mask = ""
input.each do |line|
  ops = line.split(" ")
  case ops[0]
  when "mask"
    mask = ops[2]
  when /mem\[(\d+)\]/
    value = Integer(ops[2])
    coded = ops[0].match(/^(mem\[)(\d+)(\])$/)[2]
    decoded = decodeLocations(mask, Integer(coded))
    decoded.each { |location| memory[location] = value }
  end
end

total = memory.sum { |k,v| v }
puts "Sum of all values in memory: #{total}"