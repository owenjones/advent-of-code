file = io.open("input.txt")
presents = file:lines()

total = 0
for present in presents do
  for l, w, h in present:gmatch("([^%D]+)x([^%D]+)x([^%D]+)") do
    extra = math.min((l*w), (w*h), (h*l))
    total = total + (2 * ((l*w) + (w*h) + (h*l))) + extra
  end
end

print("Total sq ft of wrapping paper needed", total) -- 1598415