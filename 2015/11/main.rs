use std::fs;

fn increment(password: Vec<u32>) -> Vec<u32> {
    let mut v = password.clone();
    
    for i in (0..v.len()).rev() {
        let n = v[i] + 1;
        v[i] = n % 26;
        if (n / 26) == 0 {
            return v;
        }
    }

    return v;
}

fn is_valid(password: &Vec<u32>) -> bool {
    if password.contains(&8) || password.contains(&11) || password.contains(&14) {
        return false;
    }

    let mut c = 0;
    let mut i = 0;
    while i < (password.len() - 1) {
        if password[i] == password[i+1] {
            c += 1;
            i += 2;
        }

        if c == 2 {
            break;
        }

        i += 1;
    }

    if c < 2 {
        return false;
    }

    let mut i = 0;
    while i < (password.len() - 2) {
        if password[i+1] == (password[i] + 1 % 26) && password[i+2] == (password[i] + 2 % 26) {
            return true;
        }

        i += 1;
    }
    
    return false;
}

fn v2s(v: &Vec<u32>) -> String {
    v.clone().into_iter().map(|c| char::from_u32(c + 97).unwrap()).collect()
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");
    let values: Vec<u32> = input.chars().collect::<Vec<char>>().into_iter().map(|c| (c as u32) - 97).collect();

    let mut v = values;
    loop {
        v = increment(v);

        if is_valid(&v) {
            let s: String = v2s(&v);
            println!("Next valid password: {s}");
            break;
        }
    }

    loop {
        v = increment(v);

        if is_valid(&v) {
            let s: String = v2s(&v);
            println!("Next valid password after that: {s}");
            break;
        }
    }

}
