Neighbours = [0, 1, -1].repeated_permutation(3)

def runCycle(input)
  output = Hash.new(false)

  return output
end

def countActive(cubes)
  return cubes.select { |k,v| v == true }.size
end

state = Hash.new(false)
File.open("test_input.txt") do |input|
  input.read.split("\n").each_with_index do |row,iRow|
    row.split("").each_with_index do |cell,iColumn|
      state[[iColumn, iRow, 0]] = (cell == "#")
    end
  end
end

for _ in 1..6 do
  state = runCycle(state)
end
active = countActive(state)

puts "Number of active cubes after six cycles: #{active}"