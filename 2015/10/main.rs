use std::fs;

fn look_and_say(input: String) -> String {
    // sketch - break string into groups of identical chars, count number of those and build next string
    let mut o = String::new();

    let mut p = input.chars().nth(0).unwrap();
    let mut n = 1;

    for c in input[1..input.len()].chars() {
        // println!("{c}");
        if c == p {
            n += 1;
            continue;
        }

        o.push_str(&n.to_string());
        o.push_str(&p.to_string());
        p = c;
        n = 1;
    }

    o.push_str(&n.to_string());
    o.push_str(&p.to_string());

    return o;
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");

    let mut sequence = input;
    for _ in 0..40 {
        sequence = look_and_say(sequence);
    }

    let n = sequence.len();
    println!("After 40 rounds: {n}");
}
