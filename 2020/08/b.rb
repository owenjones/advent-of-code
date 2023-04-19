class Processor
  attr_accessor :acc

  def initialize
    @acc = 0
    @iptr = 0 # instruction pointer
    @pptr = Array.new # previous instruction ptrs
    @run = true
  end

  def load(instructions)
    @instructions = instructions
    @visits = Array.new(@instructions.size) { |x| 0 }
  end

  def run

    instructions = @instructions

    # control program flow - run, catch loop, attempt fix, repeat

  end
  
  def test_cycle
    
    while true
      if @iptr > @instructions.size
        return 1 # program ran until the end!
      end

      if @visits[@iptr] == 1
        # we hit an infinite loop
        return 0
      end

      @visits[@iptr] += 1

      data = @instructions[@iptr].match(/^(\w{3}) ([+-]{1}\d+)$/)
      
      instruction = data[1]
      value = Integer(data[2])

      @pptr.push(@iptr)
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

File.open("test_input.txt") do |input|
    processor.load(input.read.split("\n"))
end

processor.run

puts "Value in accumulator at execution finish: #{processor.acc}"