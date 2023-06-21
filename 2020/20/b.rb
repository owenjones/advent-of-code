raw = File.open("test_input.txt").read.split("\n\n")

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

types = Hash.new(false)
tiles.each { |id, tile| types[id] = tile.map { |t| t.map { |e| counts[e] }.sum }.min }

# from part 1 we can identify corners (6 edge matches), sides (7 edge matches),
# and internal pieces (8 edge matches)

corners = types.select { |k, v| v == 6 }.keys # 4
sides = types.select { |k, v| v == 7 }.keys   # 40
insides = types.select { |k, v| v == 8 }.keys # 100

# possible rough approach - pick a corner, try and work through the sides to another corner
# work around the outside (like a jigsaw puzzle) building up the border,
# then fill in from the top row-by-row. Memoise each layout of tiles, if we hit
# a point where we cannot place a tile then we have an invalid layout: dump and continue
# with the next one?

puts tiles[corners[0]].to_s