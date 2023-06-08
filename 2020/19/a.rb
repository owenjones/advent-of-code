def expandRules(rules)
  expanded = Hash.new
  rules.each do |rule|
    m = rule.match(/(\d+): (.*)/)
    expanded[Integer(m[1])] = m[2].sub("\"a\"", "a").sub("\"b\"", "b")
  end
  return expanded
end

def generateRegex(rules)
  while (n = rules[0].match(/(\d)+/)) != nil
    r = ["(", rules[Integer(n[1])], ")"].join
    rules[0].sub!(n[0], r)
  end

  rules[0].gsub!(/\s+/, "")
  return /^#{rules[0]}$/
end

(rules, messages) = File.open("test_input.txt").read.split("\n\n").map { |x| x.split("\n") }
rules = expandRules(rules)
puts rules.inspect
expression = generateRegex(rules)
matches = messages.map { |x| x.match?(expression) }.select { |x| x == true }.size
puts "Number of messages which match rule 0: #{matches}"