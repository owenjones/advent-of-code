use std::cmp::Ordering;
use std::collections::{BTreeSet, BinaryHeap, HashMap};
use std::fs;

#[derive(Clone, Debug, Eq, PartialEq)]
struct Search<'a> {
    cost: u32,
    position: &'a str,
    nodes: BTreeSet<&'a str>,
}

impl Ord for Search<'_> {
    fn cmp(&self, other: &Self) -> Ordering {
        other
            .cost
            .cmp(&self.cost)
            .then_with(|| other.position.cmp(&self.position))
    }
}

impl PartialOrd for Search<'_> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

fn find_shortest(nodes: &BTreeSet<&str>, edges: &HashMap<(&str, &str), u32>) -> Option<u32> {
    let mut heap: BinaryHeap<Search> = BinaryHeap::new();

    for node in nodes {
        let mut search = Search {
            cost: 0,
            nodes: nodes.clone(),
            position: node,
        };
        search.nodes.remove(node);
        heap.push(search);
    }

    loop {
        let s = heap.pop();
        if s.is_some() {
            let s = s.unwrap();
            if s.nodes.len() == 0 {
                return Some(s.cost);
            }

            for n in s.nodes.iter() {
                let mut ns = s.clone();
                let c = edges.get(&(ns.position, n));

                if c.is_some() {
                    ns.nodes.remove(n);
                    ns.cost += c.unwrap();
                    ns.position = n;
                    heap.push(ns);
                }
            }
        } else {
            break;
        }
    }

    return None;
}

fn find_longest(nodes: &BTreeSet<&str>, edges: &HashMap<(&str, &str), u32>) -> Option<u32> {
    let mut heap: BinaryHeap<Search> = BinaryHeap::new();

    for node in nodes {
        let mut search = Search {
            cost: 0,
            nodes: nodes.clone(),
            position: node,
        };
        search.nodes.remove(node);
        heap.push(search);
    }

    let mut costs: Vec<u32> = Vec::new();

    loop {
        let s = heap.pop();
        if s.is_some() {
            let s = s.unwrap();
            if s.nodes.len() == 0 {
                costs.push(s.cost);
                continue;
            }

            for n in s.nodes.iter() {
                let mut ns = s.clone();
                let c = edges.get(&(ns.position, n));

                if c.is_some() {
                    ns.nodes.remove(n);
                    ns.cost += c.unwrap();
                    ns.position = n;
                    heap.push(ns);
                }
            }
        } else {
            break;
        }
    }

    return costs.iter().max().copied();
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Input not found");
    let mut nodes: BTreeSet<&str> = BTreeSet::new();
    let mut edges: HashMap<(&str, &str), u32> = HashMap::new();

    for line in input.split("\n") {
        let a: Vec<&str> = line.split(" to ").collect();
        let b: Vec<&str> = a[1].split(" = ").collect();
        let d: u32 = b[1].parse().expect("NaN");
        nodes.insert(&a[0]);
        nodes.insert(&b[0]);
        edges.insert((&a[0], &b[0]), d);
        edges.insert((&b[0], &a[0]), d);
    }

    let p1 = find_shortest(&nodes, &edges);
    if p1.is_some() {
        println!("Shortest route: {}", p1.unwrap());
    }

    let p2 = find_longest(&nodes, &edges);
    if p2.is_some() {
        println!("Longest route: {}", p2.unwrap());
    }
}
