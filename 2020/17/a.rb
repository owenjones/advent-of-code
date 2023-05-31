Neighbours = [0, 1, -1].repeated_permutation(3).to_a
Neighbours.delete([0, 0, 0])

def runCycle(input, size, cycle)
  space = Hash.new(false)

  # pad the cube-space by 1
  input.select { |k,v| v == true }.each do |k,v|
    space[[k[0] + 1, k[1] + 1, k[2] + 1]] = true
  end

  # calculate new cube-space range
  range = size + (cycle * 2)
  
  # for each cube in this cube space, find number of active neighbours
  output = Hash.new(false)
  for z in 0..range
    for y in 0..range
      for x in 0..range
        position = [x, y, z]
        c = 0
        Neighbours.each do |n|
          c += (space[position.zip(n).map(&:sum)] == true) ? 1 : 0
        end
        
        if space[position] # cube is active
          output[position] = !(c < 2 || c > 3)
        else # cube is inactive
          output[position] = (c == 3)
        end
      end
    end
  end

  puts "Cycle #{cycle}, active cubes = #{countActive(output)}"
  return output
end

def countActive(cubes)
  return cubes.select { |k,v| v == true }.size
end

size = 0 # initial size of cube-space
state = Hash.new(false)
File.open("input.txt") do |input|
  input.read.split("\n").each_with_index do |row,r|
    row.split("").each_with_index do |cell,c|
      if cell == "#"
        state[[c, r, 0]] = true
      end
      size = c
    end
  end
end
size += 1

for i in 1..6 do
  state = runCycle(state, size, i)
end