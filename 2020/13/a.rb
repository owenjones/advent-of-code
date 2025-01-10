def findBus(time, buses)
  # for (time, time+1, time+2...) check if any of the bus IDs are a factor
  # for first match return dt * bus ID
  t = time
  while true
    buses.each do |bus|
      if (t % bus) == 0
        return (t - time) * bus
      end
    end
    t += 1
  end
end

note = File.open("input.txt").read.split
time = Integer(note[0])
buses = note[1].split(",").reject { |x| x == "x" }.map { |x| Integer(x) }
result = findBus(time, buses)
puts "Time difference multiplied by bus ID: #{result}"