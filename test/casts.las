function split(string: String, delimiter: Char) ~> [String] {
    let result = [];
    let current = "";
    for char in string {
        if char == delimiter {
            result += [current];
            current = "";
            continue;
        }
        current = current + char;
    }
    if current.length > 0 {
        result += [current];
    }
    return result;
}

let s = "100
200";
let parts = split(s, '\n');
for part in parts {
    let number = part => I32;
    let result = number * 2;
    println(number + " * 2 = " + 2 * number);
    let string_result = result => String;
    println(string_result + " (" + typeof(string_result) + ")");
}

assert(true => String == "true");
assert(false => String == "false");
assert(42 => String == "42");
assert('!' => String == "!");
assert((1..10) => String == "1..10");
