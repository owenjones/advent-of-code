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
    ticket = ticket.split(",")
    valid = ticket.map { |n| numbers.include?(Integer(n)) }.select { |x| x == true }.size
    (valid == ticket.size) ? ticket : nil
  end
  return tickets.compact
end

def testPermutation(permutation, tickets, fields)
  tickets.each do |ticket|
    ticket.each.with_index do |n,i| 
      return nil if !fields[permutation[i]].include?(Integer(n))
    end
  end
  return permutation
end

def matchFields(valid, fields)
  permutations = fields.map { |k, v| k }.permutation
  permutations.each do |permutation|
    t = testPermutation(permutation, valid, fields)
    next if t == nil
    return permutation
  end
end

def extractDepartureFields(ticket, fields)
  numbers = ticket.split("\n")[1].split(",")
  matched = fields.zip(numbers).select { |x| x[0].match?(/^departure (.*)$/) }
  summed = matched.map { |x| Integer(x[1]) }
  return summed
end

blocks = File.open("input.txt").read.split("\n\n")
fields = parseFields(blocks[0])
valid = findValid(blocks[2], fields)
matched = matchFields(valid, fields)
departureValues = extractDepartureFields(blocks[1], matched)
answer = departureValues.inject(:*)
puts "Product of the six departure values: #{answer}"

# 1995808337179 - too high