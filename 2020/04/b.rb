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

    def validByr
        if @byr.nil?
            return false
        end

        byr = Integer(@byr)
        return @byr.size == 4 && byr >= 1920 && byr <= 2002
    end

    def validIyr
        if @iyr.nil?
            return false
        end

        iyr = Integer(@iyr)
        return @iyr.size == 4 && iyr >= 2010 && iyr <= 2020
    end

    def validEyr
        if @eyr.nil?
            return false
        end

        eyr = Integer(@eyr)
        return @eyr.size == 4 && eyr >= 2020 && eyr <= 2030
    end

    def validHgt
        if @hgt.nil?
            return false
        end

        match = @hgt.match(/(?<height>\d+)(?<unit>\D{2})/)

        if match.nil?
            return false
        end

        height = Integer(match[:height])

        if match[:unit] == "cm" && height >= 150 && height <= 193
            return true
        elsif match[:unit] == "in" && height >= 59 && height <= 76
            return true
        else
            return false
        end
    end

    def validHcl
        if @hcl.nil?
            return false
        end

        return @hcl.match?(/#[0-9a-f]{6}/)
    end

    def validEcl
        if @ecl.nil?
            return false
        end

        colors = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]
        return colors.include?(@ecl)
    end

    def validPid
        if @pid.nil?
            return false
        end

        return @pid.match?(/^\d{9}$/)
    end

    def isValid
        return validByr && validIyr && validEyr && validHgt && validHcl && validEcl && validPid
    end
end

passports = Array.new

File.open("input.txt") do |input|
    passports = input.read.split("\n\n")
end

valid = 0

passports.each do |passport|
    p = Passport.new(passport)
    valid += (p.isValid ? 1 : 0)
end

puts "Valid passports: #{valid}"
