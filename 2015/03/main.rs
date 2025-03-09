use std::fs;
use std::collections::HashSet;

#[derive(Hash, Eq, PartialEq, Debug, Copy, Clone)]
struct P {
  x: i32,
  y: i32
}

fn part1(input: &String) -> i32 {
  let mut p = P { x: 0, y: 0 };
  let mut visited: HashSet<P> = HashSet::new();

  visited.insert(p);
  for c in input.chars() {
    match c {
      '>' => p.x += 1,
      '<' => p.x -= 1,
      '^' => p.y += 1,
      'v' => p.y -= 1,
      _ => panic!("unknown")
    }
    visited.insert(p);
  }

  return visited.iter().count() as i32;
}

fn part2(input: &String) -> i32 {
  let mut s = P { x: 0, y: 0 };
  let mut r = P { x: 0, y: 0 };
  let mut visited: HashSet<P> = HashSet::new();

  visited.insert(s);
  for (i, c) in input.chars().enumerate() {
    let a = if i % 2 == 0 { &mut s } else { &mut r };

    match c {
      '>' => a.x += 1,
      '<' => a.x -= 1,
      '^' => a.y += 1,
      'v' => a.y -= 1,
      _ => panic!("unknown")
    }

    visited.insert(*a);
  }

  return visited.iter().count() as i32;
}

fn main() {
  let input = fs::read_to_string("input.txt").expect("Input not found");
  
  let p1 = part1(&input);
  println!("Part 1: {p1}");

  let p2 = part2(&input);
  println!("Part 2: {p2}");
}