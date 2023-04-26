def saidOnTurn(numbers, stop)
  lastSaid = Hash.new(nil)
  numbers.each_with_index { |n,i| lastSaid[n] = [i+1] }
  turn = numbers.size + 1
  said = numbers.last
  while turn <= stop
    if lastSaid[said].size == 1
      said = 0
    else
      n = lastSaid[said][-2, 2]
      said = n[1] - n[0]
    end
    if lastSaid[said] == nil
      lastSaid[said] = Array.new
    end
    lastSaid[said].push(turn)
    turn += 1
  end
  return said
end

numbers = File.open("input.txt").read.split(",").map { |x| Integer(x) }
said = saidOnTurn(numbers, 2020)
puts "The number said on turn 2020: #{said}"
