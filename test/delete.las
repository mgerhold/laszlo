function remove_if(array_or_string: ?, predicate: Function(?) ~> Bool) {
    let i = 0;
    while i < array_or_string.size {
        if predicate(array_or_string[i]) {
            array_or_string.delete(i);
        } else {
            i += 1;
        }
    }
}

function is_odd(n: I32) ~> Bool {
    return n mod 2 != 0;
}

function iota(n: I32) ~> [I32] {
    let result = [];
    for i in 0..n {
        result += [i];
    }
    return result;
}

function is_vowel(char: Char) ~> Bool {
    return char == 'a' or char == 'e' or char == 'i' or char == 'o' or char == 'u';
}

function main() {
    let numbers = iota(42);
    remove_if(numbers, is_odd);
    println(numbers);

    let text = "This is a text with a lot of different letter.";
    remove_if(text, is_vowel);
    println(text);
}

main();
