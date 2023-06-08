def evalAdvancedExpression(expression)
  # split expression into parts, take every + and evaluate with numbers either side, then reinsert answer
  parts = expression.split
  
  while parts.include?("+")
    loc = parts.find_index("+") - 1
    expr = parts.slice!(loc, 3).join
    answer = eval(expr).to_s
    parts.insert(loc, answer)
  end
 
  # should be left with an expression only containing integers and multiplications that we can simply eval
  return eval(parts.join).to_s
end

def solve(sum)
  # iteratively find expressions in set of parenthesis and evaluate, then replace in string
  while sum.include?("(") do 
    match = sum.match(/\(([\d [+*]?]+\d?)\)/)
    answer = evalAdvancedExpression(match[1])
    sum.sub!(match[0], answer)
  end

  # should be left with a string with just integers and operators we can evaluate
  return evalAdvancedExpression(sum).to_i
end

lines = File.open("input.txt").read.split("\n")
answer = lines.map { |l| solve(l) }
# puts "Answers: #{answer.to_s}"
puts "Sum of values: #{answer.sum}" # 171259538712010

# Test set answers: 231, 51, 46, 1445, 669060, 23340 (sum is 694173)