players = File.open("input.txt").read.split("\n\n").map { |raw| raw.split("\n").drop(1).map { |n| n.to_i } }

while(players[0].count > 0 && players[1].count > 0)
  a = players[0].shift
  b = players[1].shift

  if(a > b) 
    players[0].append(a)
    players[0].append(b)
  else
    players[1].append(b)
    players[1].append(a)
  end
end

winner = players[0].count > 0 ? 0 : 1;
score = players[winner].map.with_index{ |h, i| h * (players[winner].count - i) }.sum

puts "Winning score: #{score}" # 32366