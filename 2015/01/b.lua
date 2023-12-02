file = io.open("input.txt")
chars = file:read()
file:close()

floor = 0
for i = 1, #chars do
  c = chars:sub(i,i)
  if c == "(" then floor = floor + 1 end
  if c == ")" then floor = floor - 1 end
  if floor == -1 then
    print("Reached the basement at character ", i) -- 1771
    break
  end
end