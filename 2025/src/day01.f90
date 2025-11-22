module day01
    use aoc_lib, only: load_input
    use M_strings, only: split
    implicit none

    private
    public :: main

contains
    subroutine main
        character(len=:), allocatable :: input, array(:)

        input = load_input("inputs/day01.txt")
        call split(input, array, delimiters=achar(10))
        write(*,*) size(array)
        print*, array(1)

    end subroutine main
end module day01
