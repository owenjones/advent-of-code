use std::fs;
use regex::Regex;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");
    let re = Regex::new(r#"-?\d+"#).unwrap();
    let numbers: Vec<_> = re.find_iter(&input).map(|n| n.as_str().parse::<i32>().expect("NaN")).collect();

    println!("Numbers: {numbers:?}");

    let s: i32 = numbers.into_iter().sum();
    println!("Sum of all numbers: {s}");
}