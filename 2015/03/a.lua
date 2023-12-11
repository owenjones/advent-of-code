file = io.open("input.txt")
chars = file:read()
file:close()

x = 0
y = 0
visited = { ["0:0"] = true }
houses = 1
for i = 1, #chars do
  c = chars:sub(i,i)
  if c == ">" then x = x + 1
  elseif c == "<" then x = x - 1
  elseif c == "^" then y = y - 1
  elseif c == "v" then y = y + 1
  end

  if visited[x .. ":" .. y] == nil then
    houses = houses + 1
    visited[x .. ":" .. y] = true
  end
end

print("Houses visited:", houses)