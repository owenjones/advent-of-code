class Processor
  attr_accessor :acc

  def load(instructions)
    @instructions = instructions
  end

  def decorrupt
    dptr = 0

    while true
      instructions = @instructions.dup
      i = instructions[dptr].match(/^(\w{3}) ([+-]{1}\d+)$/)

      case i[1]
      when "acc"
        dptr += 1
        next
      when "jmp"
        instructions[dptr] = "nop " + i[2]
        dptr += 1
      when "nop"
        instructions[dptr] = "jmp " + i[2]
        dptr += 1
      end
      
      outcome = run(instructions)
 
      if outcome == 1
        return
      end
    end
  end
  
  def run(instructions)
    @acc = 0
    iptr = 0
    visits = Array.new(instructions.size) { |x| 0 }

    while true
      if iptr >= instructions.size
        return 1 # program ran until the end, hurray!
      end

      if visits[iptr] == 1
        # we hit an infinite loop
        return 0
      end

      visits[iptr] += 1

      data = instructions[iptr].match(/^(\w{3}) ([+-]{1}\d+)$/)
      
      instruction = data[1]
      value = Integer(data[2])

      case instruction
      when "nop"
        iptr += 1
      when "acc"
        @acc += value
        iptr += 1
      when "jmp"
        iptr = iptr + value
      end
    end
  end
end

processor = Processor.new
processor.load(File.open("input.txt").read.split("\n"))
processor.decorrupt

puts "Value in accumulator at execution finish: #{processor.acc}"