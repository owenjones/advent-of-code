def parseFields(fields)
  fields.split("\n").map do |field|
    n = field.match(/^([\w: ]+) ([\d]+)-([\d]+) or ([\d]+)-([\d]+)$/)
    [(Integer(n[2])..Integer(n[3])).to_a + (Integer(n[4])..Integer(n[5])).to_a]
  end
end

def testNearby(nearby, fields)
  numbers = fields.flatten.uniq
  invalid = nearby.split("\n").drop(1).map do |ticket|
    ticket.split(",").map { |n| !numbers.include?(Integer(n)) ? Integer(n) : 0 }.sum
  end
  return invalid
end

blocks = File.open("input.txt").read.split("\n\n")
fields = parseFields(blocks[0])
invalid = testNearby(blocks[2], fields)
rate = invalid.sum
puts "Ticket scanning error rate: #{rate}"