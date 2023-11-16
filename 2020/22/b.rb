require 'set'

class Game
  def initialize(hands)
    @hands = hands
    @cache = Set.new
    @winner = false
  end

  def round
    if (@cache.include?(@hands.to_s))
      # game ends if we've seen this permutation of cards before (player 0 wins)
      @winner = 0
      return
    end

    if(@hands[0].count == 0)
      # game ends if player 0 has no cards left (player 1 wins)
      @winner = 1
      return
    end
    
    if(@hands[1].count == 0)
      # game ends if player 1 has no cards left (player 0 wins)
      @winner = 0
      return
    end

    @cache.add(@hands.to_s)
    a = @hands[0].shift
    b = @hands[1].shift

    if(a <= @hands[0].count && b <= @hands[1].count)
      # both players have at least as many cards remaining as number drawn - enter recursive combat
      hands = [@hands[0].first(a), @hands[1].first(b)]
      winner = Game.new(hands).play
    elsif(a > b)
      winner = 0
    elsif(b > a)
      winner = 1
    end

    # place cards in winners hand ready for next round
    if(winner == 0)
      @hands[0].append(a)
      @hands[0].append(b)
    else
      @hands[1].append(b)
      @hands[1].append(a)
    end
  end

  def play
    while(@winner == false)
      round
    end

    return @winner
  end

  def score()
    n = @hands[@winner].count
    return @hands[@winner].map.with_index{ |h, i| h * (n - i) }.sum
  end
end

openingHands = File.open("input.txt").read.split("\n\n").map{ |raw| raw.split("\n").drop(1).map{ |n| n.to_i } }
game = Game.new(openingHands)
game.play
puts "Winning score: #{game.score}"