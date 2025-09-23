use std::fs;
use regex::Regex;
use serde_json::Value;

fn jsonsum(v: &Value) -> i64 {
    if v.is_object() {
        let mut s: i64 = 0;
        for (_, v) in v.as_object().unwrap().into_iter() {
            if v == "red" {
                return 0;
            } else {
                s += jsonsum(v);
            }
        }
        return s;
    }

    if v.is_array() {
        return v.as_array().unwrap().into_iter().map(|n| jsonsum(n)).into_iter().sum();
    }

    if v.is_number() {
        return v.as_i64().expect("NaN");
    }

    return 0;
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");

    let inner = Regex::new(r#"(-?\d+)"#).unwrap();
    let numbers: Vec<i32> = inner.find_iter(&input).map(|n| n.as_str().parse::<i32>().expect("NaN")).collect();
    let s: i32 = numbers.into_iter().sum();
    println!("Sum of all numbers: {s}");

    let v: Value = serde_json::from_str(&input).unwrap();
    let s: i64 = jsonsum(&v);
    println!("Sum of all numbers that aren't in red objects: {s}");
}