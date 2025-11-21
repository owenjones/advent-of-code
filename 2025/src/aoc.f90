module aoc
  implicit none

  private
  public :: load_input
  
contains
  function load_input(file) result(buffer)
        character(len=*), intent(in) :: file
        character(len=:), allocatable :: buffer
        integer :: io, stat, length

        open(&
            file=file, &
            newunit=io, &
            iostat=stat, &
            action="read", &
            access="stream", &
            form="unformatted" &
        )

        if(stat /= 0) stop "Failed to open '" // file // "'"

        inquire(io, size=length)
        allocate(character(len=length) :: buffer)
        read(io, iostat=stat) buffer
        close(io)

    end function load_input
end module aoc
