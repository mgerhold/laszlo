function create_array(n: I32) ~> [I32] {
    let result = [];
    for i in 1..=n {
        result += [i];
    }
    return result;
}

function get_array_creator() ~> Function(I32) ~> [I32] {
    return create_array;
}

function f() ~> String {
    let split = "1 2 3".split(' '); // shadowing the split function right after using it

    function merge(array: [String]) ~> String {
        return array.join("_-_");
    }

    return split.merge();
}

println("a b c".split(' ', true));
println(split("a b c", ' ', true));
assert([1, 2, 3].size == 3);
println([1, 2, 3].join(", "));
println([1, 2, 3].join('|'));
println(get_array_creator()(42).join(", "));

println(f());

println("a b c".split(' ', true));
println("1 2  3 4  5 6".split(' '));
println("1 2  3 4  5 6".split(' ', false));
println("1 2  3 4  5 6".split(' ', true));
