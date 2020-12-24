use std::collections::HashMap;

// A single move where input[0] is always the current cup
// Additionally, note that since the numbers are from 1 onwards
// the smallest number is always 0.
//
// (Try not to cringe at the number of heap allocs)
fn part_one_inner(input: Vec<u64>) -> Option<Vec<u64>> {
    let max = input.iter().max().copied()?;
    let min = 0; // the numbers are always consecutive and from 1.
    let curr = input[0];
    let sel: &[u64] = &input[1..4];

    // sort out the destination
    let mut dest = input[0] - 1;
    if dest == min {
        dest = max;
    }
    while sel.contains(&dest) {
        dest -= 1;
        if dest == min {
            dest = max;
        }
    }
    let dest_pos = input.iter().position(|&x| x == dest)?;

    // split and reassemble chunks to do the array shuffle
    let (mut first, second): (Vec<u64>, Vec<u64>) = {
        (
            input[0..dest_pos].iter().copied().skip(4).collect(),
            input[dest_pos + 1..].iter().copied().collect(),
        )
    };

    first.push(dest);
    first.extend_from_slice(sel);
    first.extend_from_slice(second.as_slice());
    first.push(curr);

    Some(first)
}

// this works for part 1 as well. i had to redo this because the existing solution
// to part 1 wouldn't work for part 2 (too slow)
fn part_two_inner(input: Vec<u64>) -> Option<u64> {
    let max = input.iter().max().copied()?;
    let min = 0; // the numbers are always consecutive and from 1.
    let mut curr = input[0];

    // maps each i'th index of the input to it's neighbors
    // (basically simulating a linkedlist)
    //
    // maybe this is faster if you just use a vector such that
    // the label with the i'th index points to the index of the clockwise neighbor?
    // It would still be ~O(1) with O(n) space, and you'd _maybe_ get better cache locality.
    let mut neighbors = HashMap::new();
    for i in 0..input.len() {
        neighbors.insert(input[i], input[(i + 1) % input.len()]);
    }

    for _ in 0..10_000_000 {
        let sel_1 = *neighbors.get(&curr)?;
        let sel_2 = *neighbors.get(&sel_1)?;
        let sel_3 = *neighbors.get(&sel_2)?;
        let nb = *neighbors.get(&sel_3)?;
        neighbors.insert(curr, nb);

        // sort out the destination
        let mut dest = curr - 1;
        if dest == min {
            dest = max;
        }
        while [sel_1, sel_2, sel_3].contains(&dest) {
            dest -= 1;
            if dest == min {
                dest = max;
            }
        }

        // do the splicing business
        let curr_dest_neighbor = *neighbors.get(&dest)?;
        neighbors.insert(dest, sel_1);
        neighbors.insert(sel_1, sel_2);
        neighbors.insert(sel_2, sel_3);
        neighbors.insert(sel_3, curr_dest_neighbor);

        // next iter with new curr
        curr = *neighbors.get(&curr)?;
    }

    let first = *neighbors.get(&1)?;
    let second = *neighbors.get(&first)?;

    Some(first * second)
}

fn part_one(mut input: Vec<u64>) {
    for _ in 0..100 {
        input = part_one_inner(input).unwrap();
    }

    let ans = input
        .iter()
        .map(|&x| x.to_string())
        .collect::<Vec<String>>()
        .join("");
    println!("Part one answer: {}", ans);
}

fn part_two(mut input: Vec<u64>) {
    for i in 1_000_000 + 1 {
        input.push(i);
    }

    let answer = part_two_inner(input).unwrap();
    println!("Part two answer: {}", answer);
}

fn main() {
    let input: Vec<u64> = [1, 3, 5, 4, 6, 8, 7, 2, 9].to_vec();

    part_one(input.clone());
    part_two(input);
}
