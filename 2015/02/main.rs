use std::fs;

fn main() {
  let input = fs::read_to_string("input.txt").expect("Input not found");
  let mut wrapping = 0;
  let mut ribbon = 0;

  for present in input.lines() {
    let x: Vec<&str> = present.split("x").collect();
    let y: Vec<i32> = x.iter().map(|v| v.parse::<i32>().unwrap()).collect();

    let dim = [y[0] * y[1], y[1] * y[2], y[2] * y[0]];
    let extra = dim.iter().min().unwrap();
    wrapping += 2 * ((y[0] * y[1]) + (y[1] * y[2]) + (y[2] * y[0])) + extra;

    let sides = [(2 * y[0] + 2 * y[1]),(2 * y[1] + 2 * y[2]), (2 * y[0] + 2 * y[2])];
    let extra = sides.iter().min().unwrap();
    ribbon += (y[0] * y[1] * y[2]) + extra;
  }

  println!("Part 1: {wrapping}");
  println!("Part 2: {ribbon}");
}