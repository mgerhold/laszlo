struct S {
    name: String,
    age: I32,
}

let s = new S {
    name: "Claus Kleber",
    age: 67,
};

let s2 = new S {
    name: "Gundula Gause",
    age: 58,
};

let persons = [
        new S {
            name: "Claus Kleber",
            age: 67,
        },
        new S {
            name: "Gundula Gause",
            age: 58,
        }];

println(typeof(s));
println(s);
println(s == s);
println(s == s2);
println(s.name);
s.name = "Claus-Detlev Walter Kleber";
println(s);

persons[0].name = "Claus-Detlev Walter Kleber";
println(persons[0]);
