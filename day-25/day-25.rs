// https://rob.co.bb/posts/2019-02-10-modular-exponentiation-in-rust/
fn mod_pow(mut base: u64, mut exp: u64, modulus: u64) -> u64 {
    if modulus == 1 { return 0 }
    let mut result = 1;
    base = base % modulus;
    while exp > 0 {
        if exp % 2 == 1 {
            result = result * base % modulus;
        }
        exp = exp >> 1;
        base = base * base % modulus
    }
    result
}

fn main() {
    let calc = |num: u64| {
        for i in 0..100_000_000 {
            if mod_pow(7, i, 20201227) == num {
                return i;
            }
        }

        unreachable!();
    };

    let answer = mod_pow(6929599, calc(2448427), 20201227);
    dbg!(answer);
}
