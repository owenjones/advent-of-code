module day01
    use aoc, only: load_input
    implicit none

    private
    public :: run

contains
    subroutine run
        character(len=:), allocatable :: input

        input = load_input("inputs/day01.txt")
        print "(A)", input
    end subroutine run
end module day01
