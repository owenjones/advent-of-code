def babyStepGiantStep(alpha, beta, order)
  # Use Baby-step Giant-step algorithm to solve DLP (https://en.wikipedia.org/wiki/Baby-step_giant-step)
  m = Math.sqrt(order).ceil

  # Baby-step
  alpha_j = Hash.new(false)
  (0..m).map { |j| alpha_j[alpha.pow(j, order)] = j }

  # Giant-step
  alpha_m = alpha.pow((m * (order - 2)), order)
  for i in (0..m)
    x = (beta * alpha_m.pow(i, order)) % order 
    if alpha_j[x] != false
      return i * m + alpha_j[x]
    end
  end
end

(cardKey, doorKey) = File.open("input.txt").read.split("\n").map(&:to_i)

doorLoops = babyStepGiantStep(7, doorKey, 20201227) # 7^x ≡ doorKey (mod 20201227) -> solve for x
encryptionKey = cardKey.pow(doorLoops, 20201227)

puts "Encryption key: #{encryptionKey}" # 18433997