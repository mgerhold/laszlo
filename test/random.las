let seed = 42;
let num_dice_rolls = 600;
let dice_min_value = 1;
let dice_max_value = 6;

let modulus = 65536;
let a = 25213;
let c = 11;
let current = seed;

let next = 0;
let results = [];
for _ in 0..num_dice_rolls {
    current = (a * current + c) mod modulus;
    results = results + [current mod dice_max_value + 1];
}

let total = 0;
for dice_roll_result in dice_min_value..=dice_max_value {
    let count = 0;
    for j in 0..num_dice_rolls {
        if results[j] == dice_roll_result {
            count = count + 1;
            total = total + 1;
        }
    }
    println(dice_roll_result + ": " + count);
}
println("total: " + total);
