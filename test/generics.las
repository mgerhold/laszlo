function my_join(array: [?], separator: String) ~> String {
    let result = "";
    if array.size > 0 {
        result = result + array[0];
    }
    for i in 1..array.size {
        result = result + separator + array[i];
    }
    return result;
}

println(my_join([1, 2, 3], ", "));
println(my_join(["test", "sentence", "with", "words"], ":"));
println(my_join([[1, 2, 3], [4, 5, 6], [7, 8, 9]], " <-> "));

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

struct GenericStruct {
    number: I32,
    value: ?,
}

struct Person {
    name: String,
    age: I32,
}

let s = new GenericStruct{
    number: 42,
    value: new Person{
        name: "Claus Kleber",
        age: 67,
    }
};

println(s);