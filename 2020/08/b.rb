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

  def decorrupt
    run(@instructions) # we know this will loop, but it'll give us the execution order
    order = @pptr.clone
    dptr = order.size - 1 # start at end of execution order

    while true
      if dptr < 0
        puts "Gone back too far"
        exit
      end

      instructions = @instructions.clone
      i = instructions[order[dptr]].match(/^(\w{3}) ([+-]{1}\d+)$/)

      case i[1]
      when "acc"
        dptr -= 1
        next
      when "jmp"
        instructions[order[dptr]] = "nop " + i[2]
      when "nop"
        instructions[order[dptr]] = "jmp " + i[2]
      end

      @acc = 0
      @iptr = 0
      @pptr = Array.new
      @visits = Array.new(@instructions.size) { |x| 0 }

      outcome = run(instructions)

      if outcome == 1
        return
      else
        instructions = @instructions
        dptr -= 1
      end
    end


  end
  
  def run(instructions)
    while true
      if @iptr >= @instructions.size
        return 1 # program ran until the end!
      end

      if @visits[@iptr] == 1
        # we hit an infinite loop
        return 0
      end

      @visits[@iptr] += 1

      data = instructions[@iptr].match(/^(\w{3}) ([+-]{1}\d+)$/)
      
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

processor.decorrupt

puts "Value in accumulator at execution finish: #{processor.acc}"