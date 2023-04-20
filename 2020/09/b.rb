def findTarget(numbers, preambleLength)
  for i in preambleLength..(numbers.size-1)
    preamble = numbers.slice((i-preambleLength), preambleLength)
    invalid = 0
    preamble.each do |n|
      remainder = (numbers[i] - n).abs
      next if n == remainder
      break if preamble.include?(remainder)
      invalid += 1
      if invalid == preambleLength
        return numbers[i]
      end
    end
  end
end

def findWeakness(numbers, target)
  # start by cutting the set of numbers down - remove everything larger than the target (inclusive) -- next step would make that redundent though?
  # beginning at first number, step through adjacent numbers summing until either we hit target size, or are above it
  # move to next number (if takes too long could possibly implement a jump by looking at numbers in the set?)

  numbers = numbers.slice(0, numbers.index(target))
  skip = 0
  numbers.each do |a|
    skip += 1
    set = [a]
    total = a

    search = numbers.slice(skip, numbers.size)
    search.each do |b|
      total += b
      set.push(b)

      if total == target
        return set.min + set.max
      end

      break if total > target
    end

    next if total > target
  end
end

numbers = Array.new
File.open("input.txt") do |input|
  numbers = input.read.split("\n").map { |x| Integer(x) }
end

target = findTarget(numbers, 25)
weakness = findWeakness(numbers, target)

puts "Encryption weakness: #{weakness}"