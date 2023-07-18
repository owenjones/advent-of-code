foods = File.open("input.txt").read.split("\n")

allIngredients = Array.new
allAllergens = Array.new
appearences = Hash.new(0)
candidates = Hash.new

foods.each do |food|
  match = food.match(/^([\w ]+)\(contains ([\w, ]+)\)$/)
  ingredients = match[1].split
  allergens = match[2].split(", ")

  allergens.each do |allergen|
    candidates[allergen] = Array.new if candidates[allergen] == nil
    candidates[allergen] << ingredients
  end

  ingredients.each { |ingredient| appearences[ingredient] += 1}

  allIngredients << ingredients
  allAllergens << allergens
end

allIngredients.flatten!.uniq!
allAllergens.flatten!.uniq!

allAllergens.each do |allergen|
  candidates[allergen] = candidates[allergen].inject(:&)
end

candidates.flat_map { |k,v| v }.uniq.each do |candidate|
  allIngredients.delete(candidate)
end

count = allIngredients.map { |i| appearences[i] }.sum
puts "Ingredients without allergens appear #{count} times" # 2150