use std::collections::HashMap;
use std::fs;

fn get_value<'a>(wire: &'a str, wires: &HashMap<&str, &'a str>, cache: &mut HashMap<&'a str, u16>) -> Option<u16> {
    let i = wire.parse::<u16>();
    if i.is_ok() {
        return Some(i.unwrap());
    }
    
    if cache.contains_key(wire) {
        return cache.get(wire).copied();
    }

    // statement will be either a string key for another wire, a string representation of a number,
    // "NOT <wire>", or "<wire> AND/OR/LSHIFT/RSHIFT <wire/value>"
    // need to recursively walk the statements until we can deduce a numeric value for a wire
    // -> implement some caching of finalised values to speed up
    let statement: Vec<&str> = wires.get(wire)?.split(" ").collect();
    if statement.len() == 1 {
        let num = statement[0].parse::<u16>();
        if num.is_ok() {
            cache.insert(wire, num.clone().unwrap());
            return Some(num.unwrap());
        }
        else {
            return get_value(wires.get(wire)?, wires, cache);
        }
    }
    else if statement[0] == "NOT" {
        let value:u16 = !get_value(statement[1], wires, cache).unwrap();
        cache.insert(wire, value.clone());
        return Some(value);
    }
    else {
        let value = match statement[1] {
            "OR" => get_value(statement[0], wires, cache).unwrap() | get_value(statement[2], wires, cache).unwrap(),
            "AND" => get_value(statement[0], wires, cache).unwrap() & get_value(statement[2], wires, cache).unwrap(),
            "LSHIFT" => get_value(statement[0], wires, cache).unwrap() << statement[2].parse::<u16>().unwrap(),
            "RSHIFT" => get_value(statement[0], wires, cache).unwrap() >> statement[2].parse::<u16>().unwrap(),
            _ => panic!("Unknown operation '{}'", statement[1])
        };

        cache.insert(wire, value.clone());
        return Some(value);
    }
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");
    let mut wires: HashMap<&str, &str> = HashMap::new();

    for line in input.split("\n").collect::<Vec<&str>>() {
        let i = line.split(" -> ").collect::<Vec<&str>>();
        wires.insert(i[1], i[0]);
    }

    // Part 1
    let mut cache: HashMap<&str, u16> = HashMap::new();
    let p1 = get_value("a", &wires, &mut cache).unwrap();
    println!("Part 1: {p1}");

    // Part 2
    cache = HashMap::new();
    let b = p1.to_string();
    wires.insert("b", &b);
    let p2 = get_value("a", &wires, &mut cache).unwrap();
    println!("Part 2: {p2}");
}