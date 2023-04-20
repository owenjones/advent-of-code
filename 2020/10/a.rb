numbers = Array.new
File.open("input.txt") do |input|
  numbers = input.read.split.map { |x| Integer(x) }
end

RATING = numbers.max + 3

# this is essentially a big pathfinding problem?
# starting at 0 find which of +1 +2 +3 are possible in list, then start a search from each of these
# drop any searches where there isn't a possible next adapter left
# if we hit device rating but there are still adapters remaining we drop that search also -- this can't happen as rating is max + 3...?
# then we should get left with one complete set that finishes at the rating?

class Search

  def initialize(visited, remaining)
    @visited = visited
    @remaining = remaining
    @possible = []

    joltSteps = [(@visited.last+1), (@visited.last+2), (@visited.last+3)]
    @options = joltSteps.map { |p| @remaining.include?(p) }
    if @options.count(true) > 0
      @possible = joltSteps.zip(@options).select { |x, p| p == true }
    end
  end

  def finished?
    return (@visited.last + 3 == RATING) && (@remaining.size == 0)
  end

  def possible?
    return @possible.size > 0
  end

  def next
    searches = Array.new
    @possible.each do |possible|
      visited = @visited.dup.push(possible[0])
      remaining = @remaining.dup
      remaining.delete_at(remaining.index(possible[0]))
      searches.push(Search.new(visited, remaining))
    end
    return searches
  end

  def calculateJoltDifferences
    ones = 0
    threes = 0
    @visited.push(RATING) # to cover final +3 jolt difference
    @visited.each_cons(2) do |n|
      diff = n[1] - n[0]
      if diff == 1
        ones += 1
      elsif diff == 3
        threes += 1
      end
    end
    return ones * threes
  end

end

searches = Array.new
searches.push(Search.new([0], numbers))
while searches.size > 0
  search = searches.pop()
  # puts searches.size
  if search.finished?
    puts "Got down to a single set..."
    differences = search.calculateJoltDifferences
    puts "Total jolt differences: #{differences}"
    break
  end

  if search.possible?
    searches.concat(search.next)
  end
end