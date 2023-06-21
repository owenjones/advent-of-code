foods = File.open("input.txt").read.split("\n")

ingredients = Array.new
allergens = Array.new

foods.each do |food|
  match = food.match(/^([\w ]+)\(contains ([\w, ]+)\)$/)
  i = match[1].split
  a = match[2].split(", ")

  # need to build sets(?) of which potential ingredients have which allergen, so we can
  # pick these apart - can we count occurances? if an ingredient appears 5 times it should be against
  # the same allergen for each

  ingredients << i
  allergens << a
end

# collapse both to get full lists of ingredients and allergens
ingredients.flatten!.uniq!
allergens.flatten!.uniq!
