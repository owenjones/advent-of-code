def parseFields(fields)
  fields = fields.split("\n")
  
end

def parseNearby(nearby)
  
end

blocks = File.open("test_input.txt").read.split("\n\n")

fields = parseFields(blocks[0])
otherTickets = parseNearby(blocks[2])