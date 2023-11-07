println(typeof("Test"));
println(typeof(42));
println(typeof(true));
println(typeof(1..10));
println(typeof([1, 2, 3]));
println(typeof([]));
println(typeof(["Hello", "World"]));

for word in ["first", "second"] {
    println(typeof(word));
}

println(typeof([] + [1, 2, 3]));
println(typeof([] + []));
println(typeof([1, 2, 3] + []));
