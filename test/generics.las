function join(array: [?], separator: String) ~> String {
    let result = "";
    if array.size > 0 {
        result = result + array[0];
    }
    for i in 1..array.size {
        result = result + separator + array[i];
    }
    return result;
}

println(join([1, 2, 3], ", "));
println(join(["test", "sentence", "with", "words"], ":"));
println(join([[1, 2, 3], [4, 5, 6], [7, 8, 9]], " <-> "));

function add(lhs: ?, rhs: ?) ~> ? {
    return lhs + rhs;
}

let a = add(1, 2);
let b = add("Hello, ", "World!");
let c = add("The answer is ", 42);

println(a);
assert(typeof(a) == "I32");
println(b);
assert(typeof(b) == "String");
println(c);
assert(typeof(c) == "String");
assert(typeof(add) == "Function(?, ?) ~> ?");

function f(n: I32) {
    let a = n;
    a = a + 1;
}

let words = ["a", "b"];
f(words.size);
