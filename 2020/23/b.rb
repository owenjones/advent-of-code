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

  def initialize(array)
    @head = nil
    @tail = nil
    @index = Hash.new
    insertArray(array)
    @tail.next = @head # close the loop
  end

  def insertArray(labels)
    labels.each do |label|
      node = Node.new(label)
      @index[label] = node

      if @head == nil
        @head = node
        @tail = node
      else
        @tail.next = node
        @tail = node
      end
    end
  end

  def insert(nodes, at:)
    after = at.next
    at.next = nodes[0]
    nodes[-1].next = after
  end

  def take(m, after:)
    start = after
    nodes = Array.new
    (1..m).each do
      nodes.append(after.next)
      after = after.next
    end
    start.next = after.next
    return nodes
  end

  def find(label)
    return @index[label]
  end
end

labels = File.open("test_input.txt").read.split("").map{ |x| x.to_i }
labels += ((labels.max + 1)..(1000000)).to_a
cups = LinkedList.new(labels)

cursor = cups.head
(1..10000000).each do
  c = cups.take(3, after: cursor)

  destination = cursor.data
  while true
    destination -= 1
    destination = (destination == 0) ? 1000000 : destination
    if !c.map{ |x| x.data }.include?(destination)
      break
    end
  end

  cups.insert(c, at: cups.find(destination))
  cursor = cursor.next
end

position = cups.find(1)
answer = position.next.data * position.next.next.data
puts "Answer: #{answer}"