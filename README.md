# TesseraC Compiler
A compiler for simple programing language Ash (i created it), written in C++ for ARM64 (will support x86_64 in future version)

It tokenizes, parses and compiles  syntax directly into native Apple Silicon (ARM64) assembly



# Motive
- i built a CHIP-8 Emulator in C++, and i liked it very much it was very different from the Web Development that i have been doing all the time
- so this time i decided to go more into low level, my senior (3rd year CS), gave me his spare book and guess the title, Compiler Design and Operating System
- i gave it a read and i went all in, hail low level



# Features 
### Implementation of Compiler Design
- Lexical Analyzer
- Syntax Analyzer
- Semantic Analyzer
- Code Generator
- Symbol Table
- Error Handler


## Programing Language: Ash
- Static Typed
- Procedural
- operators support -> Arithmetic, Logical
- if-else ladder
- while loop
- int , float, string, boolean
- local and global Variables
- Functions, return values and arguments
- print keyword to print to console





## Syntax
`
int a = 5;
print a

int b = a + 10;
print b;
`



`
fn sum(int a, int b) {
    return a+b;
}

print fn(10, 20);
`
