def decodeLocations(mask, coded)
  mask = mask.split("")
  bits = coded.to_s(2)
  padded = (("0" * (mask.size - bits.size)) + bits).split("")
  locations = [[]]
  mask.zip(padded).each do |pair|
    case pair[0]
    when "0" then locations.each { |l| l.push(pair[1]) }
    when "1" then locations.each { |l| l.push("1") }
    when "X"
      n = Array.new
      locations.each do |l|
        n.push(l.dup)
        n.last.push(0)
        n.push(l.dup)
        n.last.push(1)
      end
      locations = n
    end
  end
  return locations.map { |l| l.join.to_i(2) }
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
    value = Integer(ops[2])
    coded = ops[0].match(/^(mem\[)(\d+)(\])$/)[2]
    decoded = decodeLocations(mask, Integer(coded))
    decoded.each { |location| memory[location] = value }
  end
end

total = memory.sum { |k,v| v }
puts "Sum of all values in memory: #{total}"