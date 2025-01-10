class RuleList
    def initialize
        @bags = Hash.new
    end

    def addRule(rule)
        ruleSplit = rule.sub(/\./, "").match(/(?<parent>[a-z ]+) bags contain (?<children>.*)/)
        children = ruleSplit[:children].split(", ")

        if @bags[ruleSplit[:parent]].nil?
            @bags[ruleSplit[:parent]] = Hash.new
        end

        children.each do |child|
            childSplit = child.match(/(?<count>\d+) (?<type>[a-z ]+) bag([s]?)/)
            if !childSplit.nil?
                @bags[ruleSplit[:parent]][childSplit[:type]] = Integer(childSplit[:count])
            end
        end
    end

    def getShinyGoldBagCount
        def getChildCount(parent)
            if parent.size == 0
                return 0
            end

            totalBags = 0
            parent.each do | child, count |
                numberOfBags = count + count * getChildCount(@bags[child])
                totalBags += numberOfBags
            end
            return totalBags
        end

        return getChildCount(@bags["shiny gold"])
    end
end

rules = RuleList.new
File.open("input.txt").read.split("\n").map { |r| rules.addRule(r) }

puts "Number of bags inside shiny gold bag: #{rules.getShinyGoldBagCount}"