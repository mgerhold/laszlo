let numbers = [1, 2, 3];
println(numbers); // [1, 2, 3]
for i in 0..3 {
    numbers[i] = numbers[i] * 2;
}
println(numbers); // [2, 4, 6]

let words = ["some", "words"];
println(words); // [some, words]
words[1] = "stuff";
println(words); // [some, stuff]

let to_be_mutated = ["Hello, ", "Greetings, ", "Good day, "];
for word in to_be_mutated {
    word = word + "Santa Claus!";
}
println(to_be_mutated);
