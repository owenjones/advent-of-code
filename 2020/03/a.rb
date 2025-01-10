class Forest
    def readString(input)
        @trees = input.split
        @height = @trees.size
        @width = @trees[0].size
    end

    def reachedEnd(y)
        return y >= @height
    end

    def treeAt(x, y)
        return @trees[y][(x % @width)] == "#"
    end
end

forest = Forest.new
forest.readString(File.open("input.txt").read)

x = 3
y = 1
trees = 0

while !forest.reachedEnd(y) do
    trees += (forest.treeAt(x, y) ? 1 : 0)
    x += 3
    y += 1
end

puts "Number of trees encountered: #{trees}"