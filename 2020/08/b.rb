class Processor
  attr_accessor :acc

  def initialize
    @acc = 0
    @iptr = 0 # instruction pointer
    @pptr = Array.new # previous instruction ptrs
  end

  def load(instructions)
    @instructions = instructions
    @visits = Array.new(@instructions.size) { |x| 0 }
  end

  def run
    while true
      if @iptr > @instructions.size
        # program ran until the end!
        return
      end

      if @visits[@iptr] == 1
        # we hit an infinite loop
        puts "Looped initiated by instruction #{@pptr.last} (#{@instructions[@pptr.last]})"
        break

        # need to step backwards through the execution order and try swapping jmp/nop - if we hit
        # loop again we need to reset instructions and try next jmp/nop back
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