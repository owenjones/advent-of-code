x = 0
y = 0
h = 90

File.open("input.txt") do |input|
    input.read.split.each do |instruction|
        action = instruction[0]
        amount = Integer(instruction[1..])

        case action
        when "N"
            y += amount
        when "S"
            y -= amount
        when "E"
            x += amount
        when "W"
            x -= amount
        when "L"
            h = (h - amount) % 360
        when "R"
            h = (h + amount) % 360
        when "F"
            case h
            when 0..89
                y += amount
            when 90..179
                x += amount
            when 180..269
                y -= amount
            when 270..359
                x -= amount
            end
        end
    end
end

distance = x.abs + y.abs
puts "Manhattan distance between start and end: #{distance}"