file = io.open("input.txt")
chars = file:read()
file:close()

floor = 0
for i = 1, #chars do
  c = chars:sub(i,i)
  if c == "(" then floor = floor + 1 end
  if c == ")" then floor = floor - 1 end
end

print("Ended up at floor ", floor) -- 138