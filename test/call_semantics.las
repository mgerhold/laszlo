function twice(numbers: [I32]) {
    for number in numbers {
        number = number * 2;
    }
}

let numbers = [1, 2, 3];
twice(numbers);
assert(numbers == [2, 4, 6]);

function replace(array: [String]) {
    array = ["replaced", "words"];
}

let words = ["hello", "world", "!"];
replace(words);
assert(words == ["replaced", "words"]);

function sum(a: I32, b: I32) ~> I32 {
    return a + b;
}
assert(sum(40, 2) == 42);
assert(sum(words.size, 40) == 42);
