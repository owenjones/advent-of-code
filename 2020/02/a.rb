valid = Array.new

File.open("input.txt").each_line do |line|
  e = line.match(/(?<min>\d+)-(?<max>\d+) (?<char>\w{1}): (?<password>\w+)/)
  count = e[:password].count(e[:char])

  if count >= Integer(e[:min]) && count <= Integer(e[:max])
    valid.push(line)
  end
end

puts "Number of valid passwords: #{valid.length}"
