def expandRules(rules)
  expanded = Hash.new(false)
  rules.each do |rule|
    m = rule.match(/(\d+): (.*)/)
    expanded[Integer(m[1])] = m[2]
  end
  return expanded
end

def generateRegex(rules)
  
end

(rules, messages) = File.open("test_input.txt").read.split("\n\n").map { |x| x.split("\n") }
rules = expandRules(rules)
expression = generateRegex(rules)
matches = messages.map { |x| x.match?(expression) }.select { |x| x == true }.size
puts "Number of messages which match rule 0: #{matches}"