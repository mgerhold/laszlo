function outer() {
    let number = 40;

    // nested functions
    function inner() {
        number += 2;
    }

    inner();

    println(number);
}

function get() ~> ? {
    // function-local struct
    struct Person {
        name: String,
        age: I32,
    }

    let person = new Person{
        name: "Claus Kleber",
        age: 67,
    };

    return person;
}

function main() {
    outer();

    println(get().name);
}

main();
