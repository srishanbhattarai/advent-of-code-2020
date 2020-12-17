use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::mem;

#[derive(PartialEq, Eq, Hash, Debug)]
struct Point(i64, i64, i64);

impl Point {
    // Gets all adjacent point to current point.
    // The size of the result vector is always 26.
    fn get_neighbors(&self) -> Vec<Point> {
        let mut adj = Vec::new();
        for dx in -1..2 {
            for dy in -1..2 {
                for dz in -1..2 {
                    if dx == 0 && dy == 0 && dz == 0 {
                        continue;
                    }

                    adj.push(Point(self.0 + dx, self.1 + dy, self.2 + dz));
                }
            }
        }

        adj
    }

    //  True if the world state specifies this point to be active.
    fn is_active(&self, state: &State) -> bool {
        state.world.contains(self)
    }

    // Number of neighbors of self that are active.
    fn num_active_neighbors(&self, state: &State) -> i64 {
        self.get_neighbors()
            .iter()
            .filter(|nb| nb.is_active(state))
            .count() as i64
    }
}

#[derive(Debug)]
struct State {
    world: HashSet<Point>,
}

impl State {
    fn new() -> Self {
        State {
            world: HashSet::new(),
        }
    }

    fn add_active_point(&mut self, point: Point) {
        self.world.insert(point);
    }

    fn run(&mut self) {
        let mut next_state = HashSet::new();

        for point in self.world.iter() {
            for neighbor in point.get_neighbors() {
                let active_nbs = neighbor.num_active_neighbors(&self);
                if self.world.contains(&neighbor) {
                    if active_nbs == 2 || active_nbs == 3 {
                        next_state.insert(neighbor);
                    }
                } else {
                    if active_nbs == 3 {
                        next_state.insert(neighbor);
                    }
                }
            }
        }

        let _ = mem::replace(&mut self.world, next_state);
    }
}

fn read_file(f: &str) -> io::Result<Vec<Vec<char>>> {
    let reader = BufReader::new(File::open(f)?);

    let mut result: Vec<Vec<char>> = Vec::new();
    for line in reader.lines() {
        result.push(line.unwrap().chars().collect());
    }

    Ok(result)
}

fn main() -> io::Result<()> {
    let mut state = State::new();

    let mut i = 0;
    for line in read_file("input.txt")? {
        let mut j = 0;
        for ch in line {
            if ch == '#' {
                state.add_active_point(Point(i, j, 0));
            }
            j += 1;
        }
        i += 1;
    }

    (0..6).for_each(|_i| state.run());

    println!("Answer: {}", state.world.len());

    Ok(())
}
