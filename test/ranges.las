for _ in 5..5 {
    assert(false);
}

for i in 5..6 { // 5
    println(i);
}

for i in 5..=6 { // 5, 6
    println(i);
}

for i in 6..5 { // 6
    println(i);
}

for i in 6..=5 { // 6, 5
    println(i);
}
