raw = File.open("input.txt").read.split("\n\n")

tiles = Hash.new(false)
raw.each do |tile|
  lines = tile.split("\n")
  id = lines.shift.match(/Tile (\d*):/)[1].to_i
  inv = lines.map { |l| l.split("") } .transpose.map { |l| l.join }

  tiles[id] = [
    [lines[0], inv[9], lines[9], inv[0]].map { |l| l.gsub(/[.#]/, '.' => 0, '#' => 1).to_i(2) }, # normal card
    [lines[0].reverse, inv[0], lines[9].reverse, inv[9]].map { |l| l.gsub(/[.#]/, '.' => 0, '#' => 1).to_i(2) }, # flipped horizontally
    [lines[9], inv[9].reverse, lines[0], inv[0].reverse].map { |l| l.gsub(/[.#]/, '.' => 0, '#' => 1).to_i(2) }, # flipped vertically
  ]
end

counts = Hash.new(0)
tiles.map { |k, v| v.flatten.uniq }.flatten.each { |v| counts[v] += 1 }

answer = 1
tiles.each do |id, tile|
  sums = tile.map { |t| t.map { |e| counts[e] }.sum }
  if sums.min == 6
    answer *= id
  end
end

puts "Product of corner tiles: #{answer}"