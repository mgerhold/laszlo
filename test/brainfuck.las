function execute_brainfuck(source: String, memory: [I32], index: I32, start: I32, depth: I32) {
    function ensure_enough_memory() {
        while index >= memory.size {
            memory += [0];
        }
    }

    let instruction_pointer = start;
    while instruction_pointer < source.length {
        let current = source[instruction_pointer];
        assert(current != ','); // input is not supported
        if current == '>' {
            index += 1;
        } else if current == '<' {
            index -= 1;
        } else if current == '+' {
            memory[index] += 1;
        } else if current == '-' {
            memory[index] -= 1;
        } else if current == '.' {
            print((0 => Char) + memory[index]);
        } else if current == '[' {
            if memory[index] > 0 {
                execute_brainfuck(source, memory, index, instruction_pointer + 1, depth + 1);
                continue;
            } else {
                let nesting = 0;
                while instruction_pointer < source.length {
                    if source[instruction_pointer] == '[' {
                        nesting += 1;
                    } else if source[instruction_pointer] == ']' {
                        nesting -= 1;
                        if nesting == 0 {
                            break;
                        }
                    }
                    instruction_pointer += 1;
                }
                assert(instruction_pointer < source.length or nesting == 0);
            }
        } else if current == ']' {
            break;
        }
        ensure_enough_memory();
        instruction_pointer += 1;
    }
}

function main() {
    let memory = [0];
    let index = 0;
    execute_brainfuck(read("test/hello_world.b"), memory, index, 0, 0);
}

main();
