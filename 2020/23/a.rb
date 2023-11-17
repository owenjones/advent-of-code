cups = File.open("input.txt").read.split("").map{ |x| x.to_i}
current = cups.first

(1..100).each do
  idx = cups.index(current)
  take = cups.rotate!(idx + 1).shift(3)
  cups.rotate!(-idx)

  destination = current - 1
  if(destination < cups.min)
    destination = cups.max
  end

  if(take.include?(destination))
    while(take.include?(destination))
      if(destination < cups.min)
        destination = cups.max
      else
        destination -= 1
      end
    end
  end

  idx = cups.index(destination)
  cups.insert((idx + 1), take).flatten!

  current = cups[(cups.index(current) + 1) % cups.count]
end

labels = cups.rotate!(cups.index(1)).drop(1).join
puts "Labels: #{labels}" # 76952348