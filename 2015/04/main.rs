use std::fs;
use md5::{Md5, Digest};

fn answer(key: &String, search: &str) -> Option<u32> {
  for i in 1.. {
    let k = format!("{}{}", key, i.to_string());
    let mut hasher = Md5::new();
    hasher.update(&k);
    let result = format!("{:x}", hasher.finalize());
    if result.starts_with(search) {
      return Some(i);
    }
  }

  return None;
}

fn main() {
  let key = fs::read_to_string("input.txt").expect("Input not found");

  let p1 = answer(&key, "00000").unwrap();
  println!("Part 1: {p1}");

  let p2 = answer(&key, "000000").unwrap();
  println!("Part 2: {p2}");
}
