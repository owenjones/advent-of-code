file = io.open("input.txt")
chars = file:read()
file:close()

x = {0, 0}
y = {0, 0}
visited = { ["0:0"] = true }
houses = 1
for i = 1, #chars do
  s = (i % 2) + 1 -- 2 = santa, 1 = robo-santa

  c = chars:sub(i,i)
  if c == ">" then x[s] = x[s] + 1
  elseif c == "<" then x[s] = x[s] - 1
  elseif c == "^" then y[s] = y[s] - 1
  elseif c == "v" then y[s] = y[s] + 1
  end

  if visited[x[s] .. ":" .. y[s]] == nil then
    houses = houses + 1
    visited[x[s] .. ":" .. y[s]] = true
  end
end

print("Houses visited:", houses)