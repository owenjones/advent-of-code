module day01
    use aoc_lib, only: load_input
    implicit none

    private
    public :: main

contains
    subroutine main
        character(len=:), allocatable :: input
        integer :: answer

        input = load_input("inputs/day01.txt")

        answer = 0
        write(*, '(a, i0)') "Part 1: ", answer
        ! write(*, '(a, i0)') "Part 2: ", answer

    end subroutine main
end module day01
