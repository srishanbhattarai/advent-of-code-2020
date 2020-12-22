use std::collections::{HashSet, VecDeque};
use std::io::{self, Read};

fn solve_part_one(a: &mut VecDeque<usize>, b: &mut VecDeque<usize>) -> usize {
    while !a.is_empty() && !b.is_empty() {
        let p = a.pop_front().unwrap();
        let q = b.pop_front().unwrap();

        if q > p {
            b.push_back(q);
            b.push_back(p);
        } else if p > q {
            a.push_back(p);
            a.push_back(q);
        } else {
            unreachable!();
        }
    }

    // could just use .enumerate() but I'm stupid
    let sz = std::cmp::max(a.len(), b.len());
    let x = (1..sz + 1).rev();
    if a.is_empty() {
        b.iter().zip(x).map(|(&el, n)| el * n).sum()
    } else {
        a.iter().zip(x).map(|(&el, n)| el * n).sum()
    }
}

// be patient with this one; it's painfully slow. (use -O)
fn solve_part_two_inner(
    mut a: VecDeque<usize>,
    mut b: VecDeque<usize>,
) -> (usize, VecDeque<usize>) {
    let mut seen: HashSet<(VecDeque<usize>, VecDeque<usize>)> = HashSet::new();

    while !a.is_empty() && !b.is_empty() {
        if seen.contains(&(a.clone(), b.clone())) {
            return (0, a);
        }
        seen.insert((a.clone(), b.clone()));

        let p = a.pop_front().unwrap();
        let q = b.pop_front().unwrap();

        let mut winner;
        if a.len() >= p && b.len() >= q {
            let next_a = a.iter().take(p).copied().collect();
            let next_b = b.iter().take(q).copied().collect();

            winner = solve_part_two_inner(next_a, next_b).0
        } else {
            if p > q {
                winner = 0;
            } else {
                winner = 1;
            }
        }

        if winner == 0 {
            a.push_back(p);
            a.push_back(q);
        } else {
            b.push_back(q);
            b.push_back(p);
        }
    }

    if b.is_empty() {
        (0, a)
    } else {
        (1, b)
    }
}

fn solve_part_two(mut a: VecDeque<usize>, mut b: VecDeque<usize>) -> usize {
    let (winner_num, winner) = solve_part_two_inner(a, b);

    // could just use .enumerate() but I'm stupid
    let x = (1..winner.len() + 1).rev();
    winner.iter().zip(x).map(|(&el, n)| el * (n as usize)).sum()
}

fn main() -> io::Result<()> {
    let mut buf = String::new();
    io::stdin().read_to_string(&mut buf)?;
    let lines = buf.split("\n");

    let mut a = VecDeque::new();
    let mut b = VecDeque::new();

    let mut i = -1;
    for line in lines {
        if line.is_empty() {
            continue;
        }
        if line.contains("Player") {
            i += 1;
            continue;
        }
        if i == 0 {
            a.push_back(line.parse::<usize>().unwrap());
        } else if i == 1 {
            b.push_back(line.parse::<usize>().unwrap());
        }
    }

    println!(
        "Part one answer: {}",
        solve_part_one(&mut (a.clone()), &mut (b.clone()))
    );
    println!("Part two answer: {}", solve_part_two(a, b));

    Ok(())
}
