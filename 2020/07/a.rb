class RuleList
    def initialize
        @bags = Hash.new
    end

    def addRule(rule)
        ruleSplit = rule.sub(/\./, "").match(/(?<parent>[a-z ]+) bags contain (?<children>.*)/)
        children = ruleSplit[:children].split(", ")

        children.each do |child|
            childSplit = child.match(/(\d+) (?<type>[a-z ]+) bag([s]?)/)
            if !childSplit.nil?
                if @bags[childSplit[:type]].nil?
                    @bags[childSplit[:type]] = Array.new
                end

                @bags[childSplit[:type]].push(ruleSplit[:parent])
            end
        end
    end

    def getShinyGoldContainerCount
        containers = Array.new

        def getContainers(parent, containers)
            if parent.nil?
                return
            end

            parent.each do |child|
                if containers.index(child).nil?
                    containers.push(child)
                    getContainers(@bags[child], containers)
                end
            end
        end

        getContainers(@bags["shiny gold"], containers)
        return containers.size
    end
end

rules = RuleList.new
File.open("input.txt").read.split("\n").map { |r| rules.addRule(r) }

puts "Number of possible bags: #{rules.getShinyGoldContainerCount}"