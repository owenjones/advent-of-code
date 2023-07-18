foods = File.open("input.txt").read.split("\n")

allAllergens = Array.new
candidates = Hash.new

foods.each do |food|
  match = food.match(/^([\w ]+)\(contains ([\w, ]+)\)$/)
  ingredients = match[1].split
  allergens = match[2].split(", ")

  allergens.each do |allergen|
    candidates[allergen] = Array.new if candidates[allergen] == nil
    candidates[allergen] << ingredients
  end

  allAllergens << allergens
end

allAllergens.flatten!.uniq!
allAllergens.each { |allergen| candidates[allergen] = candidates[allergen].inject(:&) }

matched = Hash.new

while candidates.size > 0 do
  singles = candidates.select { |k,v| v.size == 1 }
  remainder = candidates.select { |k,v| v.size > 1 }

  singles.each do |k,v| 
    matched[k] = v
    remainder.each { |a,i| remainder[a] = i - v }
  end

  candidates = remainder
end

cDIL = allAllergens.sort.map { |a| matched[a] }.join(",") # canonical dangerous ingredient list
puts "CDIL: #{cDIL}"