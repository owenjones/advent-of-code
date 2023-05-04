def parseFields(fields)
  parsed = Hash.new
  fields.split("\n").map do |field|
    n = field.match(/^([\w ]+): ([\d]+)-([\d]+) or ([\d]+)-([\d]+)$/)
    parsed[n[1]] = (Integer(n[2])..Integer(n[3])).to_a + (Integer(n[4])..Integer(n[5])).to_a
  end
  return parsed
end

def findValid(nearby, fields)
  numbers = fields.map { |k, v| v }.flatten.uniq
  tickets = nearby.split("\n").drop(1).map do |ticket|
    ticket = ticket.split(",").map { |n| Integer(n) }
    valid = ticket.map { |n| numbers.include?(n) }.select { |x| x == true }.size
    (valid == ticket.size) ? ticket : nil
  end
  return tickets.compact
end

def matchFields(valid, fields)
  # Can we turn tickets into a matrix, then for each column find a valid field (I assume there
  # can only be one, or there could be multiple valid permutations of the fields?)
end

def extractDepartureFields(ticket, fields)
  numbers = ticket.split("\n")[1].split(",")
  matched = fields.zip(numbers).select { |x| x[0].match?(/^departure (.*)$/) }
  numbers = matched.map { |x| Integer(x[1]) }
  return numbers
end

blocks = File.open("test_input_2.txt").read.split("\n\n")
fields = parseFields(blocks[0])
valid = findValid(blocks[2], fields)

puts valid.to_s

matched = matchFields(valid, fields)
departureValues = extractDepartureFields(blocks[1], matched)
answer = departureValues.inject(:*)
puts "Product of the six departure values: #{answer}"

# 1995808337179 - too high