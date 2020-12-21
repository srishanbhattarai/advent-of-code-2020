// For part 1, there was no need to assemble the full image. We could take a look
// at each tile's borders, and then figure out how many _other_ tiles share that same
// border. Corners must have only two neighboring tiles, while other edges have 3 or
// 4 neighboring tiles. This allows us to infer the corners, using just the borders
// and multiply the respective IDs to get the answer.
//
// For part 2 there doesn't seem to be a cute trick, apart from just creating the image.

use std::collections::{HashMap, HashSet};
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::time::Instant;

type Tileset = HashMap<String, Vec<String>>;

fn main() -> io::Result<()> {
    let reader = BufReader::new(File::open("input.txt")?);

    // Input parsing - make sure to include a blank line at the end of the file.
    let mut tileset: Tileset = Default::default();
    let mut curr_tile = String::new();
    let mut tile = Vec::new();
    for line in reader.lines() {
        let line = line?;
        if line.is_empty() {
            tileset.insert(curr_tile.clone(), tile.clone());
            tile.clear();
            continue;
        }

        if line.contains("Tile") {
            curr_tile = line[5..9].to_string();
        } else {
            tile.push(line);
        }
    } // end of input parsing

    let t1 = Instant::now();

    let p1_answer = solve_part_one(&tileset);
    println!("Part 1 answer: {}", p1_answer);

    println!("Elapsed: {}ms", t1.elapsed().as_millis());

    Ok(())
}

// Given a tile, returns the 4 borders.
fn get_borders(tile: &Vec<String>) -> Vec<String> {
    let top_border = tile.first().expect("Entry must exist").to_string();
    let bot_border = tile.last().expect("Entry must exist").to_string();

    let mut left_border = String::new();
    let mut right_border = String::new();

    // concatenate the character in the first and last columns to
    // get the left and right border
    for i in 0..tile.len() {
        let ch: Vec<char> = tile[i].chars().collect();

        left_border.push(*ch.first().unwrap());
        right_border.push(*ch.last().unwrap());
    }

    vec![top_border, bot_border, left_border, right_border]
}

// Given a tileset, returns a hashmap where each key is a border
// and the value is a vector of tile numbers for which the key has a matching border.
fn get_border_matches(tileset: &Tileset) -> HashMap<String, Vec<String>> {
    let mut border_tiles: HashMap<String, Vec<String>> = HashMap::new();
    for (tile_number, tile) in tileset.iter() {
        get_borders(tile).iter().for_each(|b| {
            // since the borders can be flipped, just store the smaller
            // of the border or it's reverse. That way, any two tiles
            // which have matching borders but are flipped, will correspond
            // to the same border in the border_tiles map.
            let brev: String = b.chars().rev().collect();
            let orig_or_rev = if brev < *b { brev } else { b.to_string() };

            (*border_tiles.entry(orig_or_rev.clone()).or_default()).push(tile_number.to_string());
        });
    }

    border_tiles
}

// Given a tile, returns it's corners.
fn get_corners(tileset: &Tileset) -> Vec<String> {
    let border_tiles = get_border_matches(tileset);
    let mut counts: HashMap<String, u64> = HashMap::new();

    // all corner tiles will consist of borders not adjacent to any other edge,
    // and there will be 2 such borders for each corner tile.
    //
    // Equivalently, a corner tile has 2 neighbors, while all other tiles have 3 or 4.
    border_tiles
        .values()
        .filter_map(|v| {
            if v.len() == 1 {
                Some(v[0].clone())
            } else {
                None
            }
        })
        .for_each(|v| *counts.entry(v).or_insert(0) += 1);

    counts
        .iter()
        .filter_map(|(k, v)| if *v == 2 { Some(k) } else { None })
        .map(|k| k.to_owned())
        .collect()
}

fn solve_part_one(tileset: &Tileset) -> u64 {
    get_corners(tileset)
        .iter()
        .map(|c| c.parse::<u64>().unwrap())
        .fold(1, |acc, x| acc * x)
}

/* PART TWO SPECIFIC STUFF AFTER THIS POINT */
// I got super frustrated doing this, so I'll complete it some other day, maybe.

// sea monster
const SM: &'static str = "                  # 
#    ##    ##    ###
 #  #  #  #  #  #   ";

fn solve_part_two(tileset: &Tileset) -> u64 {
    todo!();
}
