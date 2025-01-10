class Passport
    def initialize(input)
        @byr = nil
        @iyr = nil
        @eyr = nil
        @hgt = nil
        @hcl = nil
        @ecl = nil
        @pid = nil
        @cid = nil

        fields = input.split
        fields.each do |field|
            match = field.match(/(?<key>\w+):(?<value>.+)/)
            instance_variable_set("@#{match[:key]}", match[:value])
        end
    end

    def isValid
        return !@byr.nil? && !@iyr.nil? && !@eyr.nil? && !@hgt.nil? && !@hcl.nil? && !@ecl.nil? && !@pid.nil?
    end
end

passports = File.open("input.txt").read.split("\n\n")
valid = 0

passports.each do |passport|
    p = Passport.new(passport)
    valid += (p.isValid ? 1 : 0)
end

puts "Valid passports: #{valid}"