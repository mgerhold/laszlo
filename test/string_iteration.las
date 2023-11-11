let text = "Hello, world!";
for i in 0..text.size {
    print(text[i]);
    print(" ");
}
println();

let i = 0;
for char in text {
    if i == 5 {
        text = "01234567890123456789";
    }
    if i == 10 {
        text = "ABC";
        println("for i == 10, char == " + char);
    }
    if i mod 2 == 0 {
        char = '!';
    }
    print(char + " ");
    i = i + 1;
}
println();
println(text);

for i in 0..26 {
    print('a' + i);
}
println();
