let a = 0;
let b = 1;
let num_iterations = 20;
let i = 0;

while i < num_iterations {
    if i < 9 {
        print(" ");
    }
    println((i + 1) + ". " + b);
    let next = a + b;
    a = b;
    b = next;
    i = i + 1;
}
