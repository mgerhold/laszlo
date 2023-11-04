let num_iterations = 20;

println("The first " + num_iterations + " numbers of the Fibonacci sequence:");
let a = 0;
let b = 1;
for i in 1..=num_iterations {
    if i < 10 {
        print(" ");
    }
    println(i + ". " + b);
    let next = a + b;
    a = b;
    b = next;
}
