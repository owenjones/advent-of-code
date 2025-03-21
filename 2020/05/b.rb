def getRow(str)
    # F = lower half; B = upper half
    row = [*0..127]

    str.each_char do |l|
        if l == "F"
            row = row.slice(0, row.size/2)
        else
            row = row.drop(row.size/2)
        end
    end

    return row[0]
end

def getColumn(str)
    # L = lower half; R = upper half
    column = [*0..7]

    str.each_char do |l|
        if l == "L"
            column = column.slice(0, column.size/2)
        else
            column = column.drop(column.size/2)
        end
    end

    return column[0]
end

def getID(pass)
    p = pass.match(/(?<row>\w{7})(?<column>\w{3})/)
    return (getRow(p[:row]) * 8) + getColumn(p[:column])
end

ids = File.open("input.txt").read.split.map { |ticket| getID(ticket) }

ids.sort.each do |id|
    if ids.index(id + 1).nil?
        puts "Seat ID: #{id + 1}"
        break
    end
end