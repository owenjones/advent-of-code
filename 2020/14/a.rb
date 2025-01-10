def applyMask(mask, value)
  mask = mask.split("")
  bits = value.to_s(2)
  padded = (("0" * (mask.size - bits.size)) + bits).split("")
  masked = padded.zip(mask).map { |b| (b[1] == "X") ? b[0] : b[1] }.join
  return masked.to_i(2)
end

input = File.open("input.txt").read.split("\n")
memory = Hash.new { 0 }
mask = ""
input.each do |line|
  ops = line.split(" ")
  case ops[0]
  when "mask"
    mask = ops[2]
  when /mem\[(\d+)\]/
    location = ops[0].match(/^(mem\[)(\d+)(\])$/)[2]
    memory[location] = applyMask(mask, Integer(ops[2]))
  end
end

total = memory.sum { |k,v| v }
puts "Sum of all values in memory: #{total}"