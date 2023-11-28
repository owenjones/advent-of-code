class Node
  attr_accessor :next
  attr_reader :data

  def initialize(data)
    @next = nil
    @data = data
  end
end

class LinkedList
  attr_reader :head
  attr_reader :tail
  attr_accessor :cursor

  def initialize(array)
    @head = nil
    @tail = nil
    @index = Hash.new
    insert(array)
    @tail.next = @head # close the loop
    @cursor = @head
  end

  def insert(labels, at: false)
    labels.each do |label|
      node = Node.new(label)
      @index[label] = node

      if at
        node.next = at.next
        at.next = node
      else
        if @head == nil
          @head = node
          @tail = node
        else
          @tail.next = node
          @tail = node
        end
      end
    end
  end

  def take(m)
    after = @cursor
    taken = Array.new
    (1..m).each do
      n = after.next
      taken.append(n.data)
      after.next = n.next
    end

    return taken
  end

  def find(label)
    return @index[label]
  end
end

labels = File.open("input.txt").read.split("").map{ |x| x.to_i }
labels += ((labels.max + 1)..(1000000)).to_a
cups = LinkedList.new(labels)

(1..10000000).each do
  current = cups.cursor
  c = cups.take(3)

  destination = current.data
  while true
    destination -= 1
    destination = (destination == 0) ? 1000000 : destination
    if !c.include?(destination)
      break
    end
  end

  cups.insert(c.reverse, at: cups.find(destination))
  cups.cursor = current.next
end

position = cups.find(1)
answer = position.next.data * position.next.next.data
puts "Answer: #{answer}"