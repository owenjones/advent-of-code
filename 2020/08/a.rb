class Processor
  attr_accessor :acc

  def initialize
    @acc = 0
    @iptr = 0
  end

  def load(instructions)
    @instructions = instructions
    @visits = Array.new(@instructions.size) { |x| 0 }
  end

  def run
    while true
      if @visits[@iptr] == 1
        break
      end

      @visits[@iptr] += 1

      data = @instructions[@iptr].match(/^(\w{3}) ([+-]{1}\d+)$/)
      
      instruction = data[1]
      value = Integer(data[2])

      case instruction
      when "nop"
        @iptr += 1
      when "acc"
        @acc += value
        @iptr += 1
      when "jmp"
        @iptr = (@iptr + value) % @instructions.size
      end
    end
  end
end

processor = Processor.new

File.open("input.txt") do |input|
    processor.load(input.read.split("\n"))
end

processor.run

puts "Value in accumulator at end of first loop: #{processor.acc}"