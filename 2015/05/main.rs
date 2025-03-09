use std::fs;
use std::collections::HashMap;

fn is_nice(string: &str) -> bool {
  if string.contains("ab") || string.contains("cd") || string.contains("pq") || string.contains("xy") {
    return false;
  }

  let mut letters: HashMap<char, u32> = HashMap::new();
  for c in string.chars() {
    letters.entry(c).and_modify(|x| *x += 1).or_insert(1);
  }
  let vowels = "aeiou".to_string().chars().map(|x| letters.get(&x).unwrap_or(&0)).sum::<u32>();
  
  if vowels < 3 {
    return false;
  }

  let vec = string.chars().collect::<Vec<char>>();
  for pair in vec.windows(2) {
    if pair[0] == pair[1] {
      return true;
    }
  }

  return false;
}

fn is_actually_nice(string: &str) -> bool {
  let vec = string.chars().collect::<Vec<char>>();
  let mut has_pairs = false;
  let mut has_repeat = false;

  let mut pairs: HashMap<&[char], usize> = HashMap::new();
  for (index, window) in vec.windows(2).enumerate() {
    if pairs.contains_key(window) {
      if *pairs.get(window).unwrap() < (index - 1) {
        has_pairs = true;
      }
    }
    else {
      // need to wrap this in else to only capture first instance of pair
      // (at least one input has 3 pairs that overlap)
      pairs.insert(window, index);
    }
  }

  for window in vec.windows(3) {
    if window[0] == window[2] {
      has_repeat = true;
    }
  }

  return has_pairs && has_repeat;
}

fn main() {
  let input = fs::read_to_string("input.txt").expect("Input not found");

  let p1 = input.split("\n").collect::<Vec<&str>>().iter().map(|x| is_nice(x)).filter(|x| *x).count();
  println!("Part 1: {p1}");

  let p2 = input.split("\n").collect::<Vec<&str>>().iter().map(|x| is_actually_nice(x)).filter(|x| *x).count();
  println!("Part 2: {p2}");
}
