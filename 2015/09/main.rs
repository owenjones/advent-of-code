use std::fs;
use std::cmp::Ordering;
use std::collections::{BinaryHeap, HashMap, HashSet};

#[derive(Clone, Debug, Eq, PartialEq)]
struct Search<'a> {
    cost: u32,
    position: u32,
    nodes: HashSet<&'a str>
}

impl Ord for Search<'_> {
    fn cmp(&self, other: &Self) -> Ordering {
        other.cost.cmp(&self.cost).then_with(|| self.position.cmp(&other.position))
    }
}

impl PartialOrd for Search<'_> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");
    let mut nodes: HashSet<&str> = HashSet::new();
    let mut edges: HashMap<(&str, &str), u32> = HashMap::new();
    let mut heap: BinaryHeap<Search> = BinaryHeap::new();

    for line in input.split("\n") {
        let a: Vec<&str> = line.split(" to ").collect();
        let b: Vec<&str> = a[1].split(" = ").collect();
        let d: u32 = b[1].parse().expect("NaN");
        nodes.insert(&a[0]);
        nodes.insert(&b[0]);
        edges.insert((&a[0], &b[0]), d);
        edges.insert((&b[0], &a[0]), d);
    }

    // println!("{nodes:?}");
    // println!("{edges:?}");

    for node in &nodes {
        let mut search = Search{
            cost: 0,
            nodes: nodes.clone(),
            position: 0,
        };
        search.nodes.remove(node);

        println!("{search:?}");
    }
}