valid = Array.new

File.open("input.txt") do |input|
  input.each_line do |line|
    e = line.match(/(?<min>\d+)-(?<max>\d+) (?<char>\w{1}): (?<password>\w+)/)
    first = e[:password][Integer(e[:min]) - 1] == e[:char]
    second = e[:password][Integer(e[:max]) - 1] == e[:char]

    if first ^ second
      valid.push(line)
    end
  end
end

puts "Number of valid passwords: #{valid.length}"
