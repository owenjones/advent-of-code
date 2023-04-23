def findInverse(a)
  # find inverse multiplicative x_i, given N_i * x_i ≡ 1 (mod n_i)
  # input: a = [N_i, n_i]
  (1..a[1]).each { |x| break x if (a[0] * x) % a[1] == 1}
end

def findSequenceStart(buses)
  # for all buses in a valid sequence starting at time t: (busID - index) mod busID ≡ t
  # implementation of Chinese Remainder Theorem to solve for t

  b = Array.new # b = remainders
  n = Array.new # n = moduli

  buses.each_with_index do |bus,index|
    if bus != "x"
      bus = Integer(bus)
      b.push((bus - index) % bus)
      n.push(bus)
    end
  end

  pn = n.reduce(&:*) # N = product of all n
  ni = n.map { |x| pn / x } # N_i = N/n_i
  xi = ni.zip(n).map { |x| findInverse(x) } # x_i = inverse of N_i
  s = b.zip(ni, xi).map { |x| x.reduce(&:*) } # b_i * N_i * x_i for all i
  return s.sum % pn # t = sum of all b_i/N_i/x_i products, mod N
end

note = Array.new
File.open("input.txt") do |input|
  note = input.read.split
end

buses = note[1].split(",")
time = findSequenceStart(buses)
puts "Start time of first sequence of bus departures: #{time}"