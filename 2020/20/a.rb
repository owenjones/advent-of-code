def loadTiles(file)
  # test input is a 9 tiles (3x3 image), puzzle input will be 144 tiles (12x12 image)
  raw = File.open(file).read.split("\n\n")
  
  tiles = Hash.new(false)
  raw.each do |tile|
    lines = tile.split("\n")
    id = lines.shift.match(/Tile (\d*):/)[1].to_i
    inv = lines.map { |l| l.split("") } .transpose.map { |l| l.join }

    tiles[id] = [
      [lines[0], inv[9], lines[9], inv[0]].map { |l| l.gsub(/[.#]/, '.' => 0, '#' => 1).to_i(2) }, # normal card
      [lines[0].reverse, inv[0], lines[9].reverse, inv[9]].map { |l| l.gsub(/[.#]/, '.' => 0, '#' => 1).to_i(2) }, # flipped horizontally
      [lines[9], inv[9].reverse, lines[0], inv[0].reverse].map { |l| l.gsub(/[.#]/, '.' => 0, '#' => 1).to_i(2) }, # flipped vertically
  ].flatten.uniq

  end
  
  return tiles
end

tiles = loadTiles("test_input.txt")

counts = Hash.new(0)
tiles.map { |k, v| v }.flatten.each { |v| counts[v] += 1 }
puts counts.sort_by(&:last).to_h.each { |k, v| puts "#{k}: \t#{v}"}