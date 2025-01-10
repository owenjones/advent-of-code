numbers = File.open("input.txt").read.split("\n").map { |x| Integer(x) }
PREAMBLE = 25
for i in PREAMBLE..(numbers.size-1)
  preamble = numbers.slice((i-PREAMBLE), PREAMBLE)
  fails = 0
  preamble.each do |n|
    remainder = (numbers[i] - n).abs
    next if n == remainder
    break if preamble.include?(remainder)
    fails += 1
    if fails == PREAMBLE
      puts "Not valid: #{numbers[i]}"
      exit
    end
  end
end