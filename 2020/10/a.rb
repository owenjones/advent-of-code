numbers = Array.new
File.open("test_input.txt") do |input|
    numbers = input.read.split.map { |x| Integer(x) }
end

deviceRating = numbers.max + 3

# this is essentially a big pathfinding problem?
# starting at 0 find which of +1 +2 +3 are possible in list, then start a search from each of these
# drop any searches where there isn't a possible next adapter left
# if we hit device rating but there are still adapters remaining we drop that search also
# then we should get left with one complete set that finishes at the rating?