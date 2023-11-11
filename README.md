[![Laszlo Logo](images/laszlo_logo.png)](https://mgerhold.github.io/laszlo/)

Laszlo is both a programming language and an interpreter that can execute Laszlo
programs.

You can try it out [right here](https://mgerhold.github.io/laszlo/).

This is just a little private project that resulted from a few hours of ~~work~~
fun. Play around with it, but don't expect much!

According to ChatGPT, Laszlo is a programming language strongly influenced by
other languages (ordered by amount of influence):

- Rust
  - `let`
  - `assert`
  - type annotations in functions
  - `~>` for return types (`->` in Rust)
- Swift
  - `let`
  - type annotations with similar syntax
  - use of ranges in `for`-loops
  - curly braces to denote code blocks
- Python
  - `assert`
  - array concatenation with `+`
  - simplicity of loops
- TypeScript
  - explicit type annotations
  - resemblance to JavaScript
- Kotlin
  - `println`
- C/C++/Java
  - block structure using `{}`
  - imperative style
  - control structures like `if` and `while`
- JavaScript
  - array operations and general "look and feel" of the code, which resembles
    modern JavaScript

I won't judge whether ChatGPT is correct or not, though ¯\\\_(ツ)_/¯

## The Laszlo Programming Language

- `print()` and `println()`: Print the string representation of something to
  stdout. `println` adds a newline. Example:
  ```
  println("Hello, world!")
  print("Hello, ");
  print("world!");
  println();
  println("The answer is " + 42);
  ```
- `function`: You can declare functions using the `function` keyword. Functions
  require type annotations (like in the
  renowned [Backseat™️](https://www.youtube.com/watch?v=dQw4w9WgXcQ) language).
  Example:
  ```
  function add(lhs: I32, rhs: I32) ~> I32 {
    return lhs + rhs;
  }
  ```
  Just like in Python, you can only call functions that have been defined
  before. If all your logic resides inside functions (except one function call
  at the end of the program), the order of function definitions doesn't matter.
  Example:
  ```
  function main() { // missing return type equals "~> Nothing"
    say_hello();
  }
  
  function say_hello() {
    println("Hello!");
  }
  
  main();
  ```
- `let`: You can create variables using `let`. All variables have to be
  initialized upon definition. Example:
  ```
  let number = 42;
  let text = "Hello!";
  let predicate = true;
  let array = [1, 2, 3];
  ```
- `=`: You can assign to variables using `=`. Example:
  ```
  let a = 10;
  a = 20;
  ```
- `if`/`else if`/`else`: They work just like in C, but you don't need
  parentheses around the condition. Example:
  ```
  let temperature = 22;
  if temperature > 25 {
    println("it's really hot");
  } else if temperature > 15 {
    println("it's warm");
  } else {
    println("it's rather cold");
  }
  ```
- `for`: Loop over an iterable. The only iterables currently available are
  ranges and arrays. Example:
  ```
  for i in 1..=10 { // will run up until 10 (inclusive)
    println(i);
  }
  for _ in 0..3 { // will run up until 2
    println("loop body");
  }
  let numbers = [1, 2, 3];
  for number in numbers {
    println(number);
  }
  ```
- `while`: Loop as long as a condition is `true`. Example:
  ```
  let i = 5;
  while i > 2 {
    println(i);
    i = i - 1;
  }
  ```
- `break`/`continue`/`return`: They work just like in C 👍.
- `assert`: Assert a certain condition. A failing assert results in aborting the
  program. Example:
  ```
  assert(42 == 42); // this is fine
  assert(false); // crashes
  ```

## Call Semantics

Laszlo has "uncommon" semantics when functions are called. In Laszlo, values are
passed "by reference" if possible ("by value" otherwise). This has some unusual
implications:

```
function twice(n: I32) {
    n = n * 2;
}

let a = 10;
twice(a);
assert(a == 20);
twice(10); // crashes, since 10 is not an Lvalue
```

If you want to take a parameter by value, you will have to store it in a new
variable. You can "re-use" the parameter name, though:

```
function twice(n: I32) {
    let n = n; // take a copy
    n = n * 2; // no effect outside this function
    println(n);
}

let a = 10;
twice(a); // prints "20"
assert(a == 10);
twice(10); // prints "20"
```

## "Type Erasure"

You can use `?` to specify the *"unspecified"* type. In function calls, `?`
matches to *any* type. Example:

```
function calculate_length(any_array: [?])~ > I32
{
  return any_array.size;
}

function print_anything(anything: ?) {
  println(anything);
}

println(calculate_length([19, 45, 7])); // prints "3"
print_anything(42); // prints "42"
calculate_length("Hello"); // crashes, type mismatch (String is not an array)
```

*Note: An empty array has the type `[?]`.*

## Now what?

Play around with the interpreter and report back any issues if you encounter
them. Solve some [Advent of Code](https://adventofcode.com/) challenges using
Laszlo! Have fun with it! Fork it! Extend it! It's a small codebase and can be a
great starting point for experiments 🙂

## FAQ

---

Q: Why is the Laszlo programming language called Laszlo?

A: I wanted some name starting with an "L", which stands for "learning" (since
almost all of my projects are learning projects). My brain decided that Laszlo
would sound cool.

---

Q: Who created the Laszlo logo?

A: ChatGPT4 (using DALL-E).

---

Q: Why does Laszlo handle XYZ in a weird way?

A: Why not?

---

Q: Why is the Laszlo interpreter so slow?

A: ¯\_(ツ)_/¯
