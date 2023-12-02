file = io.open("input.txt")
presents = file:lines()

total = 0
for present in presents do
  for l, w, h in present:gmatch("([^%D]+)x([^%D]+)x([^%D]+)") do
    sides = math.min(
      (l + h + l + h), 
      (w + h + w + h),
      (w + l + w + l)
    )
    total = total + sides + (l * w * h)
  end
end

print("Total ft of ribbon needed:", total)