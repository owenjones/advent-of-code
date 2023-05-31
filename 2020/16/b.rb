require 'set'

def parseFields(fields)
  parsed = Hash.new
  fields.split("\n").map do |field|
    n = field.match(/^([\w ]+): ([\d]+)-([\d]+) or ([\d]+)-([\d]+)$/)
    arr = (Integer(n[2])..Integer(n[3])).to_a + (Integer(n[4])..Integer(n[5])).to_a
    parsed[n[1]] = Set.new(arr)
  end
  return parsed
end

def findValid(nearby, fields)
  numbers = fields.map { |k, v| v }.inject(:|) # union individual field sets
  tickets = nearby.split("\n").drop(1).map do |ticket|
    ticket = ticket.split(",").map { |n| Integer(n) }
    valid = ticket.map { |n| numbers.include?(n) }.select { |x| x == true }.size
    (valid == ticket.size) ? ticket : nil
  end

  # transpose to get a set of numbers for each field - these are the values the field matches
  return tickets.compact.transpose.map { |s| Set.new(s.sort!) }
end

def matchFields(valid, fields)
  # find potentials (the fields that could match the values from the valid tickets)
  potentials = Array.new(valid.size) { Array.new }
  valid.each_with_index do |s,i|
    fields.each do |f|
      if f[1] >= s
        potentials[i].push(f[0])
      end
    end
  end
  
  # iteratively find entries with a single field - remove this from other entries until
  # we're left with just single fields - these are the matched fields
  while potentials.map { |a| a.size }.sum > fields.size
    singles = potentials.select { |s| s.size == 1 }.flatten
    singles.each do |single|
      potentials.map { |p| p.delete(single) if p.size > 1 }
    end
  end

  return potentials.flatten
end

def extractDepartureFields(ticket, fields)
  numbers = ticket.split("\n")[1].split(",")
  matched = fields.zip(numbers).select { |x| x[0].match?(/^departure (.*)$/) }
  numbers = matched.map { |x| Integer(x[1]) }
  return numbers
end

blocks = File.open("input.txt").read.split("\n\n")
fields = parseFields(blocks[0])
valid = findValid(blocks[2], fields)
matched = matchFields(valid, fields)
departureValues = extractDepartureFields(blocks[1], matched)
answer = departureValues.inject(:*)
puts "Product of the six departure values: #{answer}" # 1346570764607