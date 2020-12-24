use regex::Regex;
use std::collections::BTreeMap;
use std::io::{self, Read};

fn part_one(lines: Vec<String>) -> (i64, BTreeMap<(i64, i64), i64>) {
    let re = Regex::new("(e|se|sw|w|nw|ne)").unwrap();

    // stores how many times a coordinate's color has been flipped.
    // if it's odd it's currently black, otherwise it's white
    let mut num_flipped_coords = BTreeMap::new();

    for line in lines {
        if line.is_empty() { continue; }
        let (mut x, mut y) = (0, 0);
        for cap in re.captures_iter(&line) {
            let dir = cap.get(0).unwrap().as_str();

            let (dx, dy) = match dir {
                "e" => (1, 0),
                "se" => (0, -1),
                "sw" => (-1, -1),
                "w" => (-1, 0),
                "nw" => (0, 1),
                "ne" => (1, 1),
                _ => unreachable!(),
            };

            x += dx;
            y += dy;
        }

        *num_flipped_coords.entry((x, y)).or_insert(0) += 1;
    }

    let answer = num_flipped_coords
        .iter()
        .filter(|(_k, &v)| v % 2 == 1)
        .count();

    (answer as i64, num_flipped_coords)
}

// For the second parameter, the only thing that's important is the odd/evenness of the
// value not the values themselves.
//
// This took a while to figure out due to silly mistakes. I ended up
// referencing @sophiebits' code
fn part_two_inner(num_flipped_coords: BTreeMap<(i64, i64), i64>) -> BTreeMap<(i64, i64), i64> {
    // counts the number of black neighbors for each coordinate
    let mut num_black_neighbors = BTreeMap::new();
    for (&p, &count) in num_flipped_coords.iter() {
        if count % 2 == 0 {
            continue;
        } else {
            // for every neighbor of a black tile, increment their counter
            [(1, 0), (0, -1), (-1, -1), (-1, 0), (0, 1), (1, 1)]
                .iter()
                .for_each(|(dx, dy)| {
                    *num_black_neighbors.entry((p.0 + dx, p.1 + dy)).or_insert(0) += 1;
                });
        }
    }

    // basically the same as num_flipped_coords but after the rules of part 2 are applied
    let mut new_num_flipped_coords: BTreeMap<(i64, i64), i64> = BTreeMap::new();

    // for
    for (&p, &count) in num_flipped_coords.iter() {
        // this tile is currently black
        if count % 2 == 1 {
            let num = *num_black_neighbors.get(&p).unwrap_or(&0);
            if num == 0 || num > 2 {
                // flip to white
            } else {
                // stays black.
                new_num_flipped_coords.insert(p, 1);
            }
        }
    }

    // for white tiles
    for (&p, &count) in num_black_neighbors.iter() {
        if count == 2 {
            let num = *num_flipped_coords.get(&p).unwrap_or(&0);
            if num % 2 == 0 {
                // flip to black
                new_num_flipped_coords.insert(p, 1);
            }
        }
    }

    new_num_flipped_coords
}

fn part_two(mut num_flipped_coords: BTreeMap<(i64, i64), i64>) -> i64 {
    let mut v = 0;
    for _ in 0..100 {
        num_flipped_coords = part_two_inner(num_flipped_coords);
        v = num_flipped_coords
            .iter()
            .filter(|(_k, &v)| v % 2 == 1)
            .count()
             as i64;

    }

    v
}

fn main() -> io::Result<()> {
    let mut buf = String::new();
    io::stdin().read_to_string(&mut buf)?;
    let lines = buf
        .split("\n")
        .map(|x| x.to_owned())
        .collect::<Vec<String>>();

    let (answer, num_flipped_coords) = part_one(lines.clone());
    println!("Part one: {}", answer);
    println!("Part two: {}", part_two(num_flipped_coords));

    Ok(())
}
