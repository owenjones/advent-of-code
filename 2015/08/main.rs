use std::fs;
use regex::Regex;

fn decode(string: &str) -> u16 {
    let re = Regex::new(r#"\\x[0-9a-f][0-9a-f]"#).unwrap();
    let s = string.replace(r#"\\"#, "*").replace(r#"\\""#, "*").replace(r#"\""#, "*");
    let result = re.replace_all(&s, "***");
    let string_characters = result.chars().filter(|c| *c == '*').count() as u16;
    return string_characters + 2;
}

fn encode(string: &str) -> u16 {
    let result = string.replace(r#"\"#, "*").replace(r#"""#, "*");
    let string_characters = result.chars().filter(|c| *c == '*').count() as u16;
    return string_characters + 2;
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");
    let strings: Vec<&str> = input.split("\n").collect::<Vec<&str>>();

    let p1: u16 = strings.clone().into_iter().map(|s| decode(s)).sum();
    println!("Part 1: {p1}");

    let p2: u16 = strings.clone().into_iter().map(|s| encode(s)).sum();
    println!("Part 2: {p2}");
}