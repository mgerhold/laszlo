let answer = "The answer is ";
let text = 10 + " is the number ten!";
print(answer + (40 + 2) + ".");
print(answer + "not " + 40 + 2 + ".");
print(text);
let condition = false;
let second = true;
print(condition);

let n = 6;
assert(n > 5);
assert(n >= 5);
assert(n >= 6);
assert(n < 7);
assert(n <= 7);
assert(n <= 6);
assert(n == 6);
assert(n != 7);
assert(n != 5);

n = 10;
print("now n is " + n);

while n > 0 {
    print("n = " + n);
    n = n - 1;
}
print("loop ended, n = " + n);

{
    let local = 42;
    print(local);
}

if n > 10 {
    print("n is bigger than ten");
    let a = 10;
} else if n > 8 {
    print("n is bigger than eight");
    let a = 10;
} else {
    print("n is less than or equal to eight");
    let a = 10;
}
let a = 20;
print("is a equal to twenty? " + (a == 20));
