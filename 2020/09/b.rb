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

numbers = File.open("input.txt").read.split("\n").map { |x| Integer(x) }
target = findTarget(numbers, 25)
weakness = findWeakness(numbers, target)

puts "Encryption weakness: #{weakness}"