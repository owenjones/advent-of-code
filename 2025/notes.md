# Quick Reference

## Splitting input
For a newline we need to use value in [ASCII table](https://en.wikipedia.org/wiki/ASCII#/media/File:ASCII_Table_(suitable_for_printing).svg) - `achar(10)`

``` fortran
use M_strings, only: split

call split(input, array, delimiters=achar(10))
```