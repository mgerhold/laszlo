function remove_if(array: [?], predicate: Function(?) ~> Bool) {
    let i = 0;
    while i < array.size {
        if predicate(array[i]) {
            i += 1;
        } else {
            array.delete(i);
        }
    }
}

function is_even(n: I32) ~> Bool {
    return n mod 2 == 0;
}

function iota(n: I32) ~> [I32] {
    let result = [];
    for i in 0..n {
        result += [i];
    }
    return result;
}

function main() {
    let numbers = iota(42);
    remove_if(numbers, is_even);
    println(numbers);
}

main();
