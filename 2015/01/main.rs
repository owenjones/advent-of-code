use std::fs;

fn main() {
  let input = fs::read_to_string("input.txt").expect("Input not found");
  let mut floor = 0;
  let mut basement = 0;

  for (i, c) in input.chars().enumerate() {
    if c == '(' {
      floor += 1;
    }
    else if c == ')' {
      floor -= 1;
    }

    if floor == -1 && basement == 0 {
      basement = i + 1;
    }
  }

  println!("Part 1: {floor}");
  println!("Part 2: {basement}");
}