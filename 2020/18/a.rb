def evalExpression(expression)
  # split expression into parts, take first three and eval, insert result at head, repeat until size == 1
  parts = expression.split
  while parts.size > 1
    expr = parts.shift(3).join
    answer = eval(expr).to_s
    parts.unshift(answer)
  end

  return parts[0]
end

def solve(sum)
  # iteratively find expressions in a single set of parenthesis and evaluate, then replace in sum
  while sum.include?("(") do 
    match = sum.match(/\(([\d [+*]?]+\d?)\)/)
    answer = evalExpression(match[1])
    sum.sub!(match[0], answer)
  end

  # should be left with a string with just integers and operators we can evaluate
  return evalExpression(sum).to_i
end

lines = File.open("input.txt").read.split("\n")
answer = lines.map { |l| solve(l) }
# puts "Answers: #{answer.to_s}"
puts "Sum of values: #{answer.sum}" # 12918250417632

# Test set answers: 71, 51, 26, 437, 12240, 13632 (sum is 26457)