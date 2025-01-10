Checks = [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [1, -1], [-1, 1], [-1, -1]]

def countOccupied(floor)
    return floor.map { |x| x.count("#") }.sum
end

def countAdjacent(input, i, j, type)
    adjacent = 0
    Checks.each do |check|
        l = i + check[0]
        m = j + check[1]
        next if l < 0 || l > (input.size-1)
        next if m < 0 || m >= input[i].size

        while input[l][m] == "."
            l += check[0]
            m += check[1]
            break if l < 0 || l > (input.size-1)
            break if m < 0 || m >= input[i].size
        end

        next if l < 0 || l > (input.size-1)
        next if m < 0 || m >= input[i].size

        if input[l][m] == type
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
                    if count >= 5
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