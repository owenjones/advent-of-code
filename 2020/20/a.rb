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
puts counts.sort_by(&:last).to_h.reject{ |k, v| v < 2 }.each { |k, v| puts "#{k}: \t#{v}"}

# what we know so far:
# each individual 'edge', represented as an integer (taken from 10-bit representation of tile edge)
# appears at maximum twice, so tile-edge-pairs are 1-to-1 (and not multiple possible edge matches)
# we also have a number of edges appearing only once (which will be edges out of the tile space)
# - can we identify which tiles have multiple single-edges (these will be corners) which will give us part [a] at least...