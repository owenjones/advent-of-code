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

    def slopeFrom(i, j)
        x = i
        y = j
        trees = 0
    
        while !reachedEnd(y) do
            trees += (treeAt(x, y) ? 1 : 0)
            x += i
            y += j
        end
    
        return trees
    end
end

forest = Forest.new()

File.open("input.txt") do |input|
    forest.readString(input.read)
end

slopes = [[1, 1], [3, 1], [5, 1], [7, 1], [1, 2]]
trees = slopes.map { |s| forest.slopeFrom(s[0], s[1]) }
product = trees.reject(&:zero?).inject(:*)

puts "Products of trees encountered: #{product}"