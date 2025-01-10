Checks = [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [1, -1], [-1, 1], [-1, -1]]

def countOccupied(floor)
    return floor.map { |x| x.count("#") }.sum
end

def countAdjacent(input, i, j, type)
    adjacent = 0
    Checks.each do |check|
        next if i + check[0] < 0
        next if i + check[0] >= input.size
        next if j + check[1] < 0
        next if j + check[1] >= input[i].size

        if input[i + check[0]][j + check[1]] == type
            adjacent += 1
        end
    end
    return adjacent
end

def printFloor(floor)
    columns = floor.map { |x| x.join("") }
    rows = columns.join("\n")
    puts rows
    puts
end

def modelArrivals(input)
    output = Array.new
    while true
        for i in 0..(input.size-1)
            row = Array.new
            for j in 0..input[i].size
                case input[i][j]
                when "."
                    row.push(".")
                when "L"
                    count = countAdjacent(input, i, j, "#")
                    if count == 0
                        row.push("#")
                    else
                        row.push("L")
                    end
                when "#"
                    count = countAdjacent(input, i, j, "#")
                    if count >= 4
                        row.push("L")
                    else
                        row.push("#")
                    end
                end
            end
            output.push(row)
        end

        break if output == input
        input = output.dup
        output = Array.new
    end
    return output
end

floor = File.open("input.txt").read.split.map { |x| x.split("") }
final = modelArrivals(floor)
occupied = countOccupied(final)

puts "Occupied seats: #{occupied}"