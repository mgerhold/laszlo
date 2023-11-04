for i in 1..=20 {
    let divisible_by_three = (i mod 3 == 0);
    let divisible_by_five = (i mod 5 == 0);
    if divisible_by_three and divisible_by_five {
        println("fizzbuzz");
    } else if divisible_by_three {
        println("fizz");
    } else if divisible_by_five {
        println("buzz");
    } else {
        println(i);
    }
}
