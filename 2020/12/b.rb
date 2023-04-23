ship = [0, 0]
waypoint = [10, 1]

def rotate(waypoint, amount)
  rotated = Array.new
  amount = (amount < 0) ? (360 + amount) : amount
  case amount
  when 90..179
    rotated[0] = waypoint[1]
    rotated[1] = -waypoint[0]
  when 180..269
    rotated[0] = -waypoint[0]
    rotated[1] = -waypoint[1]
  when 270..359
    rotated[0] = -waypoint[1]
    rotated[1] = waypoint[0]
  end
  return rotated
end

File.open("input.txt") do |input|
  input.read.split.each do |instruction|
    action = instruction[0]
    amount = Integer(instruction[1..])

    case action
    when "N"
      waypoint[1] += amount
    when "S"
      waypoint[1] -= amount
    when "E"
      waypoint[0] += amount
    when "W"
      waypoint[0] -= amount
    when "L"
      waypoint = rotate(waypoint, -amount)
    when "R"
      waypoint = rotate(waypoint, amount)
    when "F"
      ship[0] += amount * waypoint[0]
      ship[1] += amount * waypoint[1]
    end
  end
end

distance = ship[0].abs + ship[1].abs
puts "Manhattan distance between start and end: #{distance}"