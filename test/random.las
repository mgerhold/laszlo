let seed = 42;

let modulus = 65536;
let a = 25213;
let c = 11;
let current = seed;

let next = 0;
let results = [];
for _ in 0..600 {
    current = (a * current + c) mod modulus;
    results = results + [current mod 6 + 1];
}

for dice_roll_result in 1..=6 {
    let count = 0;
    for j in 0..600 {
        if results[j] == dice_roll_result {
            count = count + 1;
        }
    }
    println(dice_roll_result + ": " + count);
}
