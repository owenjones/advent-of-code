def transform(s, l)
 return s.pow(l, 20201227)
end

def findLoops(t)
  # 7^x ≡ target (mod 20201227)
  # use baby-step giant-step algorithm to solve for x (the number of loops)

  max = Math.sqrt(20201227 - 1).ceil

  # Baby-step
  b = Hash.new(false)
  (0..max).map { |k| b[7.pow(k, 20201227)] = k }

  # Giant-step
  a = 7.pow((max * (20201227 - 2)), 20201227)
  for n in (0..max)
    x = (t * a.pow(n, 20201227)) % 20201227
    if b[x] != false
      return n * max + b[x]
    end
  end
end

(cardKey, doorKey) = File.open("input.txt").read.split("\n").map(&:to_i)

doorLoops = findLoops(doorKey)
encryptionKey = transform(cardKey, doorLoops)

puts "Encryption key: #{encryptionKey}" # 18433997